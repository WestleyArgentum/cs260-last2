#include "Precompiled.h"
#include "DataStream.h"


namespace Framework
{
  DataStream::DataStream( unsigned size ) 
    : created_(true), buffer_(new char[size]), size_(size), offset_(0) {;}

  DataStream::DataStream( char *buffer, unsigned size ) 
    : created_(false), buffer_(buffer), size_(size), offset_(0) {;}

  DataStream::~DataStream( void ) { if (created_) delete [] buffer_; }


  bool DataStream::IsGood( void ) const
  { return buffer_ && (offset_ < size_); }

  ///Reading methods.
  void DataStream::ReadInt( int& i )
  {
    if ((size_ - offset_) >= sizeof(int)) {
      memcpy(&i,buffer_ + offset_,sizeof(int));
      offset_ += sizeof(int);
    }
    else
      throw "Not enough space in buffer to read from."
  }

  void DataStream::ReadUInt( unsigned &u )
  {
    if ((size_ - offset_) >= sizeof(unsigned)) {
      memcpy(&i,buffer_ + offset_,sizeof(unsigned));
      offset_ += sizeof(int);
    }
    else
      throw "Not enough space in buffer to read from."
  }
  void DataStream::ReadBool( bool &b );
  void DataStream::ReadFloat( float &f );
  void DataStream::ReadDouble( double &d );
  void DataStream::ReadString( std::string &str );
  void DataStream::ReadLine( std::string &str );

  ///Writing methods.
  void DataStream::WriteInt( const int& i );
  void DataStream::WriteUInt( const unsigned &u );
  void DataStream::WriteBool( const bool &b );
  void DataStream::WriteFloat( const float &f );
  void DataStream::WriteDouble( const double &d );
  void DataStream::WriteString( const std::string &str );
}
