
#include "Precompiled.h"
#include "NetUtilities.h"
#include <sstream>

namespace Framework
{

  ///Default constructor simply zeros out the struct and sets the family.
  NetAddress::NetAddress( void ) : port_(0)
  {
    SecureZeroMemory( &address, sizeof(address) );
    address.sin_family = AF_INET;
  }

  NetAddress::NetAddress( const IPAddress &ip, Port port )
  {
    ip_ = ip; port_ = port;
    SecureZeroMemory( &address, sizeof(address) );
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr( ip.c_str() );
  }

  ///Comparison operators because there aren't any for sockaddr_in or sockaddr.
  bool NetAddress::operator==( const NetAddress &rhs ) const {
      return address.sin_family == rhs.address.sin_family && address.sin_port == rhs.address.sin_port &&
          address.sin_addr.s_addr == rhs.address.sin_addr.s_addr; 
  }

  bool NetAddress::operator!=( const NetAddress &rhs ) const {
      return address.sin_family != rhs.address.sin_family || address.sin_port != rhs.address.sin_port ||
          address.sin_addr.s_addr != rhs.address.sin_addr.s_addr;
  }

  ///Creates an Error from the code and formats it into a human readable string.
  Error CreateError( Error::ErrorCode code )
  {
    CHAR errstr[4096];
    int errcode = GetLastError();
    std::stringstream sstr;

    DWORD result = FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      errcode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      errstr,
      sizeof(errstr),
      NULL );

    if ( result == 0 )
    {
      sstr << "Format message failed with: #" << GetLastError();

      return Error( Error::E_Unknown, sstr.str() );
    }

      // Append the error code into the message formated from windows!
    sstr << "Error #" << errcode << ": " << errstr;

    return Error( code, sstr.str() );
  }
  
  /// Creates and throws an Error exception from the code given.
  void ThrowError(Error::ErrorCode code) throw (Error)
  {
    Error er = CreateError(code);
    throw er;
  }

}
