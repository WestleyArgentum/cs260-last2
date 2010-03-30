#pragma once

#include <string>
#include <vector>
#include <stdio.h>        // FILE
#include <sys\types.h>    // __int64
#include <sys\stat.h>     // __stat66

class FileSplitter
{
  public:
    typedef struct _stat64 FileStats;
    typedef __int64  FileSize;

    struct FileChunk
    {
      unsigned short size_;
      void *data_;
    };    // FileChunk

  public:
    FileSplitter( void );
    ~FileSplitter( void ) throw();

     // Opens the file, returns true if the file was opened sucessfully.
    bool Open( const std::string &filename, unsigned chunksize );

      // Returns if the file was opened successfully.
    bool IsOpen( void ) { return pFile_ != NULL; }

    FileStats GetStats( void ) { return stats_; }
    FileSize GetFileSize( void ) { return stats_.st_size; }

      // Keep reading in a chunk from our file. Returns false once we reached the end of the file.
    bool Read( void );

      // Get the last chunk read in.
    FileChunk& GetChunk( void );

      // Get a particular chuck specified by index.
    FileChunk& GetChunk( unsigned index );

      // Erases the file chunk at the index specified.
    void Erase( unsigned index );

      // Erases all the chunks from [begin, end)
    void Erase( unsigned begin, unsigned end );

  private:
    void* CreateChunk( void ) const;
    void RemoveChunk( void *chunk ) const;

  private:
    typedef std::vector< FileChunk > Chunks;
    Chunks chunks_;

    std::string filename_;
    FILE *pFile_;
    FileStats stats_;

    unsigned short chunksize_;
};    // FileSplitter

