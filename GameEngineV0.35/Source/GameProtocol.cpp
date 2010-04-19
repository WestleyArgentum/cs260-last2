#include "Precompiled.h"
#include "GameProtocol.h"
#include "GameMessages.h"
#include "Stats.h"

namespace Framework
{
  ///Writes the header into the buffer. Header explains the type of message.
  ///Returns the number of bytes written to the buffer so as to get an offset.
  void GameProtocol::FormatHeader( DataStream &stream ) const
  {
    ///Write the number of each type of message.
    for (unsigned type = 0; type < NMid::NumIds; ++type)
      stream.WriteData(messages[type].size());
  }

  ///Because Pepper > Salt : Salted hash added here if needed.
  //virtual void PepperMessage( DataStream &stream ) const { return 0; }


  ///Decides whether the packet is a valid message, takes care of decryption and unsalting.
  bool GameProtocol::ValidateMessage( DataStream &stream )
  {
    return true;
  }

  ///Strips the header off the message and stores any data relevant to message creation.
  ///Returns the number of bytes stripped off, so that only the message data is left.
  void GameProtocol::StripHeader( DataStream &stream )
  {
    for (unsigned type = 0; type < NMid::NumIds; ++type)
      stream.ReadData(NumMessages[type]);
  }

  ///Builds a message with the remaining data after the message has been decrypted/validated.
  int GameProtocol::BuildMessages( DataStream &stream, MessageList *messages )
  {
    unsigned num = 0;
    ///Hard coded for now...

    for (unsigned i = 0; i < NumMessages[NMid::Connection] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new ConnectionMessage();
      msg->InterpretData(stream);
      messages[NMid::Connection].push_back(msg);
      ++num;
    }

    for (unsigned i = 0; i < NumMessages[NMid::Update] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new UpdateMessage();
      msg->InterpretData(stream);
      messages[NMid::Update].push_back(msg);
      ++num;
    }

    for (unsigned i = 0; i < NumMessages[NMid::Create] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new CreateMessage();
      msg->InterpretData(stream);
      messages[NMid::Create].push_back(msg);
      ++num;
    }

    for (unsigned i = 0; i < NumMessages[NMid::Destroy] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new DestroyMessage();
      msg->InterpretData(stream);
      messages[NMid::Destroy].push_back(msg);
      ++num;
    }

    for (unsigned i = 0; i < NumMessages[NMid::Stats] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new StatsMessage();
      msg->InterpretData(stream);
      messages[NMid::Stats].push_back(msg);
      ++num;
    }
    
    for (unsigned i = 0; i < NumMessages[NMid::InputBtn] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new InputButtonMessage();
      msg->InterpretData(stream);
      messages[NMid::InputBtn].push_back(msg);
      ++num;
    }
    
    for (unsigned i = 0; i < NumMessages[NMid::PlayerId] && stream.IsReadGood(); ++i)
    {
      INetMessage *msg = new PlayerMessage();
      msg->InterpretData(stream);
      messages[NMid::PlayerId].push_back(msg);
      ++num;
    }

    //for (unsigned i = 0; i < NumMessages[NMid::KeepAlive] && stream.IsReadGood(); ++i)
    //{
    //  INetMessage *msg = new KeepAliveMessage();
    //  msg->InterpretData(stream);
    //  messages[NMid::Connection].push_back(msg);
    //  ++num;
    //}

    return num;
  }

}
