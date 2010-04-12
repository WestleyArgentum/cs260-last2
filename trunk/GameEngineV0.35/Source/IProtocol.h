#pragma once


namespace Framework
{
  class INetMessage;

  ///The protocol interface for all packet formatting. Contains a few
  ///specific methods that consolidate the process for all protocols.
  class IProtocol
  {
  protected:
    const INetMessage *msg_; ///< The message to use in the packet.

  public:
    ///Save a pointer to the message that will write itself into the packet.
    IProtocol( const INetMessage *msg ) : msg_(msg) {;}
    ///Keep all destructors virtual.
    virtual ~IProtocol( void ) {;}

    ///Formats the buffer with all the packet info it needs.
    int FormatPacket( char *buffer, unsigned size );

    ///Takes the buffer of data recieved and extracts the message data and builds a message.
    INetMessage * ExtractMessage( char *buffer, unsigned size );

  protected:
    ///Writes the header into the buffer. Header explains the type of message.
    ///Returns the number of bytes written to the buffer so as to get an offset.
    virtual int FormatHeader( char *buffer, unsigned size ) const = 0;

    ///Because Pepper > Salt : Salted hash added here if needed.
    virtual int PepperMessage( char *buffer, unsigned size ) const { return 0; }


    ///Decides whether the packet is a valid message, takes care of decryption and unsalting.
    virtual bool ValidateMessage( char *buffer, unsigned size ) = 0;

    ///Strips the header off the message and stores any data relevant to message creation.
    ///Returns the number of bytes stripped off, so that only the message data is left.
    virtual unsigned StripHeader( char *buffer, unsigned size ) = 0;
  
    ///Builds a message with the remaining data after the message has been decrypted/validated.
    virtual INetMessage * BuildMessage( char *buffer, unsigned size ) = 0;
  };

}
