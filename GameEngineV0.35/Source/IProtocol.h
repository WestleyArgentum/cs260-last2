#pragma once


namespace Framework
{
  ///Forward declaration of base message type.
  class INetMessage;

  ///The type of container messages are stored in and returned in.
  typedef std::vector<const INetMessage *> NetMessageList;

  ///A type to specify what Protocol to use when interpreting messages.
  typedef unsigned ProtocolType;

  ///The protocol interface for all packet formatting. Contains a few
  ///specific methods that consolidate the process for all protocols.
  class IProtocol
  {
  protected:
    ProtocolType pType;      ///< Acts as an identifier for the type of protocol.
    NetMessageList messages; ///< The messages to send in the packet.

  public:
    ///Save a pointer to the message that will write itself into the packet.
    IProtocol( ProtocolType type ) : pType(type) {;}
    ///Keep all destructors virtual.
    virtual ~IProtocol( void ) {;}

    ///Clears messages from internal message list.
    void ClearMessages( void );

    ///Add a message to send. Doesn't delete messages when done with them.
    int AddMessage( const INetMessage *msg );

    ///Formats the buffer with all the packet info it needs.
    int FormatPacket( char *buffer, unsigned size ) const;

    ///Takes the buffer of data received and extracts the message data and builds messages.
    NetMessageList * ExtractMessages( char *buffer, unsigned size );

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
