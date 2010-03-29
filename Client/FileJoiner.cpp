#include "FileJoiner.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileChunk Public Methods

/**************************************************************************************************/
/**************************************************************************************************/
void FileJoiner::FileChunk::SetData( const void *data, unsigned size )
{
  if (size)
  {
    if (!data_)
      data = new char[size];

    memcpy(data_, data, size);
    size_ = size;
    alive_ = true;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileJoiner::FileChunk::ClearData()
{
  if (data_)
  {
    delete data_;
    data_ = 0;
    size_ = 0;
    alive_ = false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileJoiner Public Methods


/**************************************************************************************************/
/**************************************************************************************************/
FileJoiner::FileJoiner( const std::string &file, unsigned size )
: file_(file), filesize_(size), datasize_(0), begin_(0), end_(0) 
{
   // Check if size can't be broken perfectly into correct chunk sizes.
  if (size % MAX_CHUNK_SIZE)
    nchunks_ = size/MAX_CHUNK_SIZE + 1;
  else
    nchunks_ = size/MAX_CHUNK_SIZE;

   // Make the allocation all at once now. sizeof(FileChunk) = 8,  so even with a
   // 2 GB file max size of this array could be 2 MB if the chunks are 1KB each.
  filemap_.resize(nchunks_);
}

/**************************************************************************************************/
/**************************************************************************************************/
FileJoiner::~FileJoiner( void )
{
  if (!complete_)
    ; // TODO: delete the file if it was opened and not finished!!
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileJoiner::OpenFile( void )
{
   // Attempt to open the file as a binary write file.
  fout.open(file_.c_str(), std::ios_base::out, std::ios_base::binary);
  return fout.is_open() && fout.good();
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileJoiner::SaveChunk( unsigned id, const void *data, unsigned size )
{
   // Prevent incorrect index requests.
  if (id >= nchunks_)
    throw "Invalid id.";
  else if (size > MAX_CHUNK_SIZE)
    throw "Chunk size too large.";

   // See if the chunk already exists, or has been written to the hard disk.
  if (filemap_[id].alive_ || filemap_[id].written_)
    return false;

   // Save data into the FileMap and update how much data we've saved.
  filemap_[id].SetData(data,size);
  datasize_ += size;

   // Check to see if there is a contiguous section large enough worth writing to disk.
  
  // update end_ pointer
  while (end_ < nchunks_ && filemap_[end_].alive_)
    ++end_;

   // Check if all data has arrived.
  if (datasize_ == filesize_ || end_ == nchunks_)
    complete_ = true;

  // check if the contiguous size is large enough to write.
  if ((end_ - begin_) >= MIN_WRITE_SIZE || complete_)
  {
    // write the contiguous chunks to the file and pull them out of memory
    while (begin_ != end_)
    {
      fout.write(filemap_[begin_].data_, filemap_[begin_].size_);
      filemap_[begin_].ClearData();
      filemap_[begin_].written_ = true;
      ++begin_;
    }
    if (complete_)
      fout.close();
  }

  return true;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileJoiner::IsComplete( void ) const
{
   // TODO: Check if file stuff has been written n whatnot.
  return complete_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileJoiner::Cancel( void )
{
  // TODO: Clear all data. Close & delete file.
}
