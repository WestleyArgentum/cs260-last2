#pragma once

namespace Framework
{
  ///DataStream takes a character buffer and writes and reads from it.
  ///If not passed a buffer, it will create one itself of a given size.
  ///It will not delete the buffer if it didn't allocate it itself.
  class DataStream
  {
    bool created_;   ///< True if the buffer was created by this class.
    bool writefail_; ///< The write failbit, set when a write fails.
    bool readfail_;  ///< The read failbit, set when a read fails.
    char *buffer_;   ///< The buffer to store data into.
    unsigned size_;  ///< The size of the buffer.
    unsigned write_; ///< Current writing offset into buffer.
    unsigned read_;  ///< The reading offset into buffer.

    ///Calculates and returns the number of bytes remaining in the buffer.
    unsigned WriteSpace( void ) const { return size_ - write_; }
    unsigned ReadSpace( void ) const { return size_ - read_; }

    template <typename T>
    void ReadData( T &data )
    {
      if (ReadSpace() >= sizeof(T)) {
        memcpy(&data,buffer_,sizeof(T));
        read_ += sizeof(T);
      }
      else ///If we can't read that much data, set the read failbit.
        readfail_ = true;
    }

    template <typename T>
    void WriteData( T &data )
    {
      if (WriteSpace() > sizeof(T)) {
        memcpy(buffer_,&data,sizeof(T));
        write_ += sizeof(T);
      }
      else ///If we can't write that much, set the write failbit.
        writefail_ = true;
    }

  public:
    DataStream( unsigned size );
    DataStream( char *buffer, unsigned size );
    ~DataStream( void );

    ///Returns how many bytes are stored in the stream.
    unsigned Size( void ) const;
    ///Returns the number of bytes possible to be stored in the stream.
    unsigned Capacity( void ) const;

    ///If the stream is good to write to.
    bool IsWriteGood( void ) const;
    ///If the stream is good to read from.
    bool IsReadGood( void ) const;
    ///Clears all data in the stream and resets the read/write failbits.
    void Clear( void );

    ///Returns the underlying buffer. Use with caution.
    char * GetBuffer( void );

    ///Sets the reading index. Used when writing directly to the buffer.
    void SetReadIndex( unsigned index );

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
    void WriteLine( const std::string &str );
  };
}
