/**************************************************************************************************/
/*!
@file   FileTransfer.hpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#include "Client.hpp"

#include "WindowsLibrary/CommandCenter.hpp"
#include "WindowsLibrary/FileDialogs.hpp"
#include "WindowsLibrary/ProgressBar.hpp"
#include "WindowsLibrary/Timer.hpp"
#include "WindowsLibrary/DebugDiagnostic.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileAccept Methods

/**************************************************************************************************/
/**************************************************************************************************/
FileAccept::FileAccept( const FileTransferInfo &info ) : remote_(info.udp_), id(info.id_),
from_(info.from_), file_(info.file_), size_(info.filesize_), result_(0), done_(false), fail_(false)
{
  type = TRANSFER_ACCEPT;
  thread_.Resume();
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileAccept::IsDone( void )
{
  Lock lock( mutex_ );

  return done_;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileAccept::IsFail( void )
{
  Lock lock( mutex_ );

  return fail_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::Cancel( void )
{
  Kill();
}


/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::Finish( void )
{
  ++dying_;
  thread_.WaitForDeath();
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::InitializeThread( void )
{
  char buffer[512] = {0};

  sprintf_s( buffer, sizeof(buffer),
    "Incoming file from %s.\nDid you want to accept the file transfer?", from_.c_str() );

  result_ = MessageBox( NULL, buffer, "File transfer pending...", MB_YESNO | MB_ICONINFORMATION );
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::Run( void )
{
   // overloaded operator for the address comparisons
  using NAPI::operator ==;

  if ( result_ == IDNO )
  {
      // File transfer rejected.
    CommandCenter->PostMsg("", CID_SendFileTransferInfo,
      &FileTransferInfo(id, MT_REJECT_FILE, from_,"","",0,GetSocketInfo()));
    return;
  }

  SaveFileDialog saveas( NULL );
  saveas.SetRelativeDir( "..\\Data\\dropfolder" );
    // Select the destination of where to save the file and to save the file as.
  if ( !saveas.SaveFile( file_ ) )
  {
      // User canceled to save the file at a particular location. File transfer rejected.
    CommandCenter->PostMsg("", CID_SendFileTransferInfo,
      &FileTransferInfo(id, MT_REJECT_FILE, from_,"","",0,GetSocketInfo()));
    return;
  }

   // save the new file name if they changed it.
  file_ = saveas.GetFileName();
   // put from as the sender, that way the server knows where to route it.
  FileTransferInfo info(id, MT_ACCEPT_FILE, from_, "", file_, 0, GetSocketInfo());
  CommandCenter->PostMsg("", CID_SendFileTransferInfo, &info);

  ProgressBar progress(file_);
  float chunks = float(size_) / MAX_CHUNK_SIZE;
  float recvchunks = 0;
  unsigned percent = 0;

  // start recieving file!
  FileJoiner joiner(file_, size_);
  if (!joiner.OpenFile())
  {
    // failed somehow.
    Cancel();
    // TODO: send a message informing of canceling...
    return;
  }


  //DebugPrint("RECV: Connection esablished waiting for packets... filesize: %i",size_);


  while (!IsDone() && !IsFail())
  {
    // wait for a packet to arrive. timeout after a certain time limit.
    for (Timer t; !IsFail() ;)
    {
      NAPI::NetAddress address;
      int ret = socket->RecvFrom(address);
      if (ret == 0)
      {
        //DebugPrint("RECV: Recieved no data but not SOCKET_ERROR");
        continue; // got nothing....
      }
      else if (ret != SOCKET_ERROR)
      {
        // got something, is it from the correct address?
        if (address == remote_)
        {
          if (socket->GetMsg().Type() == NAPI::PT_BROKEN_TRANSFER )
          {
            // transfer failed.
            CommandCenter->PostMsg("Transfer failed: " + file_, CID_ErrorBox);
            fail_ = true;
            break;
          }

          // correct address, hand it to the filejoiner.
          unsigned seq = socket->GetMsg().GetSEQ(), ack = socket->GetMsg().GetACK();
          //DebugPrint("RECV: Got a packet from correct address.\nSEQ= %i\nACK= %i\nSize= %i",seq,ack,socket->GetMsg().DataSize());
          if (joiner.SaveChunk(seq, socket->GetMsg().Data(), socket->GetMsg().DataSize()))
          {
            if (unsigned(++recvchunks/chunks * 100.f) > percent)
            {
              percent = unsigned(recvchunks/chunks * 100.f);
              progress.Step(); // TODO: FIX THIS!!!!
            }
          }
           // Send a packet back saying we got the packet
          socket->SendTo(remote_, NAPI::PT_DATA_PACKET, 0, 0, ack, seq + 1);

          if (joiner.IsComplete())
          {
            //DebugPrint("RECV: File joining complete.\nSize recieved: %i\nChunks recieved: %i",joiner.datasize_,joiner.end_);
            done_ = true;
          }

          break;
        }
        else
        {
          //DebugPrint("RECV: Got a packet from some other random address...");
          // throw it away
        }
      }
      if ( t.TimeElapsed() < TIMEOUT_FILE_TRANSFER )
      {
         // Tell the other side the transfer timed out.
        socket->SendTo(remote_, NAPI::PT_BROKEN_TRANSFER, 0, 0, 0, 0);
        fail_ = true;
      }
    }
  }

   // If transfer failed or was canceled, inform other side.
  if (IsFail())
    socket->SendTo(remote_, NAPI::PT_BROKEN_TRANSFER, 0, 0, 0, 0);
  //DebugPrint("RECV: Exiting run...");
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::ExitThread( void ) throw()
{
    // Setting done flag.
  Lock lock( mutex_ );

  done_ = true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::FlushThread( void )
{
  fail_ = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileSend Methods

/**************************************************************************************************/
/**************************************************************************************************/
FileSend::FileSend( const std::string &to, const std::string &from, unsigned id )
: to_(to), done_(false), fail_(false)
{
  type = TRANSFER_SEND;
  OpenFileDialog dialog( NULL );
  dialog.SetRelativeDir( "..\\Data\\dropfolder" );

  if (dialog.OpenFile())
  {
    path_ = dialog.GetFileName(); // save the file path for opening
    file_ = path_.substr(path_.find_last_of("/\\") + 1); // save the filename for other user

     // If the file can't be opened, fail and return immediately.
    if (!splitter.Open( path_, MAX_CHUNK_SIZE ))
    {
      Cancel();
      return;
    }

    FileTransferInfo info(id, MT_SEND_FILE, to, from, file_, splitter.GetFileSize(), GetSocketInfo());
    CommandCenter->PostMsg("", CID_SendFileTransferInfo, &info);

    // Wait for the response before starting the thread.
  }
  else
    Cancel();
}

/**************************************************************************************************/
/**************************************************************************************************/
FileSend::~FileSend()
{
  Cancel();
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::StartTransfer(const NAPI::NetAddress &remote)
{
   // Save the udp socket info and start the transfer.
  remote_ = remote;
  thread_.Resume();
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileSend::IsDone( void )
{
  Lock lock( mutex_ );

  return done_;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileSend::IsFail( void )
{
  Lock lock( mutex_ );

  return fail_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::Cancel( void )
{
  Kill();
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::Finish( void )
{
  ++dying_;
  thread_.WaitForDeath();
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::InitializeThread( void )
{
  // read in up to 1000 chunks into memory, file into memory and break into chunks.
  //for (unsigned count = 1000; count && splitter.Read(); --count);
  // start sending data over UDP
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::Run( void )
{
  using NAPI::operator ==;
  unsigned seq = 0, ack = 0;
  if (!splitter.Read())
    fail_ = true;

  ProgressBar progress(file_);
  float chunks = float(splitter.GetFileSize()) / MAX_CHUNK_SIZE;
  float sentchunks = 0;
  unsigned percent = 0;

  //DebugPrint("SEND: Beginning file transfer.\nSize= %i",splitter.GetFileSize());

   // while the file transfer isn't done or failed
  while (!IsDone() && !IsFail())
  {
    //DebugPrint("SEND: Grabbing chunk: %i", seq);
    FileSplitter::FileChunk &chunk = splitter.GetChunk(seq);
    for (Timer timeout; !IsFail() ;)
    {
       // Send the next packet
      int ret = socket->SendTo(remote_, NAPI::PT_DATA_PACKET, chunk.data_, chunk.size_, seq, ack);
      if (ret == SOCKET_ERROR)
      {
        // TODO: something went wrong, do something about it...
        //DebugPrint("SEND: ret == SOCKET_ERROR on SendTo.");
        fail_ = true;
      }

      NAPI::NetAddress address;
      ret = socket->RecvFrom(address);
      if (ret == 0)
      {
        // got nothing...
        //DebugPrint("SEND: Got no response. Timer= %f",timeout.TimeElapsed());
      }
      else if (ret != SOCKET_ERROR && address == remote_)
      {
        if (socket->GetMsg().Type() == NAPI::PT_DATA_PACKET)
        {
          //DebugPrint("SEND: Got a packet of the correct type.\n SEQ= %i\nACK= %i",socket->GetMsg().GetSEQ(),socket->GetMsg().GetACK());
          // erase old data and send next chunk...
          if (socket->GetMsg().GetACK() == (seq + 1))
          {
            splitter.Erase(seq);
            if (!splitter.Read())
            {
              //DebugPrint("SEND: Reached EOF.");
              // file done sending
              done_ = true;
              // let the other side know somehow.
            }
            seq = socket->GetMsg().GetACK();
            ack = socket->GetMsg().GetSEQ() + 1;
            if (unsigned(++sentchunks/chunks * 100.f) > percent)
            {
              percent = unsigned(sentchunks/chunks * 100.f);
              progress.Step();
            }
            break; // no timeout
          }
        }
        else if (socket->GetMsg().Type() == NAPI::PT_BROKEN_TRANSFER)
        {
          CommandCenter->PostMsg("Transfer failed: " + file_, CID_ErrorBox);
          fail_ = true;
          break;
          //DebugPrint("SEND: Wrong type of message recieved from correct sender...");
        }
      }
      else
      {
        //DebugPrint("SEND: Message from other random source...");
        // Wrong sender, throw packet away and continue waiting...
      }

       // Check if the connection has timed out...
      if (timeout.TimeElapsed() > TIMEOUT_FILE_TRANSFER)
      {
        //DebugPrint("SEND: Connection timed out! Time= %f\nSEQ= %i", timeout.TimeElapsed(), seq);
        CommandCenter->PostMsg("Connection timed out.", CID_ErrorBox);
        fail_ = true;
        break;
      }
    }
  }

  if (IsFail())
    CommandCenter->PostMsg("Transfer failed: " + file_, CID_ErrorBox);

  //DebugPrint("SEND: Exiting run...");

}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::ExitThread( void ) throw()
{
    // Setting done flag.
  Lock lock( mutex_ );

  done_ = true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::FlushThread( void )
{
  fail_ = true;
}
