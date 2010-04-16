#pragma once

namespace Framework
{
  ///DataStream takes a character buffer and writes and reads from it.
  ///If not passed a buffer, it will create one itself of a given size.
  ///It will not delete the buffer if it didn't allocate it itself.
  class DataStream
  {
    bool created_;    ///< True if the buffer was created by this class.
    char *buffer_;    ///< The buffer to store data into.
    unsigned size_;   ///< The size of the buffer.
    unsigned offset_; ///< Current index into buffer.
  public:
    DataStream( unsigned size );
    DataStream( char *buffer, unsigned size );
    ~DataStream( void );

    bool IsGood( void ) const;
    char *GetBuffer( void );
    void Clear( void );

    ///Reading methods.
    void ReadInt( int& i );
    void ReadUInt( unsigned &u );
    void ReadBool( bool &b );
    void ReadFloat( float &f );
    void ReadDouble( double &d );
    void ReadString( std::string &str );
    void ReadLine( std::string &str );

    ///Writing methods.
    void WriteInt( const int& i );
    void WriteUInt( const unsigned &u );
    void WriteBool( const bool &b );
    void WriteFloat( const float &f );
    void WriteDouble( const double &d );
    void WriteString( const std::string &str );
  };
}
