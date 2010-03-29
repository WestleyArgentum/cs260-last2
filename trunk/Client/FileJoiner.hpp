#ifndef FILE_JOINER_H
#define FILE_JOINER_H

#include <fstream> // ifstream
#include <string>   // string
#include <vector>   // ...

 // Maximum size of chunk of data.
const unsigned short MAX_CHUNK_SIZE = 1024; /// 1KB

 // Minimum number of contiguous chunks required to write to the disk.
const unsigned MIN_WRITE_SIZE = 1024; // 1MB

class FileJoiner
{
  // A Chunk of a file being stored in memory, or having already been written.
  // Data size can be up to MAX_CHUNK_SIZE.
  struct FileChunk
  {
    FileChunk( void ) : size_(0), alive_(false), written_(false), data_(0) {;}
    ~FileChunk( void ) { ClearData(); }

    void SetData( const void *data, unsigned size ); ///< Saves the data in memory.
    void ClearData( void );  ///< Clears the data, but keeps the object alive.

     // Data Members
    unsigned short size_; ///< Size of the data stored.
    bool alive_;          ///< If any data has been saved.
    bool written_;        ///< If the data has been written to the file or not.
    char *data_;          ///< Stores data dynamically so we don't waste space.
  };  // FileChunk

   // How the Chunks are stored in memory.
  typedef std::vector<FileChunk> FileMap;

public:
  FileJoiner( const std::string &file, unsigned size );
  ~FileJoiner( void );

   // Attempts to open the file with the requested name.
  bool OpenFile( void );
   // Adds a chunk to the virtual file.
   // Returns true if chunk is added, false if it already exists.
  bool SaveChunk( unsigned id, const void *data, unsigned size );
   // Returns true if all the file data has been recieved.
  bool IsComplete( void ) const;
   // Cancels the file joining. All data is deleted as is the file on disc.
  void Cancel( void );

private:
  FileMap filemap_;   ///< A virtual map of the file in chunks.
  std::string file_;  ///< Name of the file we're writing to.
  std::ofstream fout; ///< File writing object.
  unsigned filesize_; ///< Goal size of file.
  unsigned datasize_; ///< Current size of data brought in.
  unsigned nchunks_;  ///< Number of chunks in the filemap_.
  unsigned begin_;   ///< Beginning of the contiguous section of chunks.
  unsigned end_;     ///< End of the contiguous section of chunks.
  bool complete_;     ///< If the join process is complete.
};  // FileJoiner

#endif
