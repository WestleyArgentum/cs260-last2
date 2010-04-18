#include "Precompiled.h"
#include "DataStream.h"

namespace Framework
{
  DataStream::DataStream( unsigned size ) : created_(true), writefail_(false), 
    readfail_(false), buffer_(new char[size]), size_(size), write_(0), read_(0) {;}

  DataStream::DataStream( char *buffer, unsigned size ) : created_(false), writefail_(false),
    readfail_(false), buffer_(buffer), size_(size), write_(0), read_(0) {;}

  DataStream::~DataStream( void ) { if (created_) delete [] buffer_; }

  ///Returns how many bytes are stored in the stream.
  unsigned DataStream::Size( void ) const
  {
    return write_;
  }

  ///Returns the number of bytes possible to be stored in the stream.
  unsigned DataStream::Capacity( void ) const
  {
    return size_;
  }

  bool DataStream::IsWriteGood( void ) const
  {
    return WriteSpace() > 0 && !writefail_;
  }

  bool DataStream::IsReadGood( void ) const
  {
    return ReadSpace() > 0 && !readfail_;
  }

  void DataStream::Clear( void )
  {
    ///Reset the read/write failbits.
    writefail_ = false;
    readfail_ = false;

    ///Reset indicies for reading/writing.
    write_ = 0;
    read_ = 0;

    ///Fill the buffer with 0'z..
    memset(buffer_,0,size_);
  }

  ///Returns the underlying buffer. Use with caution.
  char * DataStream::GetBuffer( void )
  {
    return buffer_;
  }

  ///Sets the reading index. Used when writing directly to the buffer.
  void DataStream::SetReadIndex( unsigned index )
  {
    read_ = index;
  }

  ///Read method.
  void DataStream::ReadData( std::string &str )
  {
    ///Relies on there being whitespace / NULL / '\n' character.
    if (IsReadGood()) {
      str.assign(buffer_ + read_);
      read_ += str.size() + 1;
    }
  }

	///Write method.
  void DataStream::WriteData( const std::string &str )
  {
    //Make sure we have enough space for the string and null character.
    if (WriteSpace() > str.size()) {
      memcpy(buffer_ + write_, str.c_str(), str.size() + 1);
      write_ += str.size() + 1;
    }
  }
}
