#include <stdlib.h>

#include "FileSplitter.hpp"
#include "FileJoiner.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileSplitter Public Methods

/**************************************************************************************************/
/**************************************************************************************************/
FileSplitter::FileSplitter( void ) :  pFile_(NULL), chunksize_( MAX_CHUNK_SIZE )
{
}

/**************************************************************************************************/
/**************************************************************************************************/
FileSplitter::~FileSplitter( void ) throw()
{
    // Erase the all of our chunks from memory
  Erase( 0, chunks_.size() );

    // Close our file.
  if ( pFile_ )
    fclose( pFile_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileSplitter::Open( const std::string &filename, unsigned chunksize )
{
  filename_ = filename;
  chunksize_ = chunksize;
    // Cap the chunk size that we read from the disk to be the maximum chunk size we specified.
  if ( chunksize_ > MAX_CHUNK_SIZE )
  {
    chunksize_ = MAX_CHUNK_SIZE;
  }

    // Get the statistics on our file that we are going to be splitting.
  int error = _stat64( filename_.c_str(), &stats_ );

  if ( error )
  {
    return false;
  }

    // Open up the file that we are splitting to send over the network!
  pFile_ = fopen( filename_.c_str(), "rb" );

  return IsOpen();
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileSplitter::Read( void )
{
    // If we still have more of the file to read in.
  if ( !feof( pFile_ ) )
  {
    FileChunk chunk;
    chunk.data_ = CreateChunk();

    if ( chunk.data_ )
    {
        // Attempt to load in a chunk of data from our file.
      chunk.size_= fread( chunk.data_, 1, chunksize_, pFile_ );

        // Check to see if this chunk is not an empty read. (Only read in the eof)
      if ( chunk.size_ == 0 )
      {
        RemoveChunk( chunk.data_ );
        return false;                 // Finished reading in this file.
      }
      else
      {
        chunks_.push_back( chunk );   // Store file chunk read in.
        return true;                  // Successfully read in a chunk from our file.
      }
    }
    else
    {
      return false;                   // Unable to create the memory for this chunk...
    }
  }
  else
  {
    return false;                     // We have finished reading in this file!
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
FileSplitter::FileChunk& FileSplitter::GetChunk( void )
{
    // Get the last chunk that we have read in from our file.
  return GetChunk( chunks_.size() - 1 );
}

/**************************************************************************************************/
/**************************************************************************************************/
FileSplitter::FileChunk& FileSplitter::GetChunk( unsigned index )
{
  return chunks_[index];
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSplitter::Erase( unsigned index )
{
  FileChunk &chunk = GetChunk( index );

  RemoveChunk( chunk.data_ );
  chunk.data_ = NULL;
  chunk.size_ = 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSplitter::Erase( unsigned begin, unsigned end )
{
  for ( unsigned i = begin; i < end; ++i )
  {
    Erase( i );
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileSplitter Private Methods

/**************************************************************************************************/
/**************************************************************************************************/
void* FileSplitter::CreateChunk( void ) const
{
  return malloc( chunksize_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSplitter::RemoveChunk( void *chunk ) const
{
  free( chunk );
}

