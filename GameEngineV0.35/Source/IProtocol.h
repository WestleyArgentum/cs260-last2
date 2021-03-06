#pragma once

#define VALIDATION_NUMBER 0xAABBCCDD

namespace Framework
{
  ///Forward declaration of base message type.
  class INetMessage;
  class DataStream;

  ///The type of container messages are stored in and returned in.
  typedef std::vector<const INetMessage *> MessageList;

  ///A type to specify what Protocol to use when interpreting messages.
  typedef std::string ProtocolType;

  ///The protocol interface for all packet formatting. Contains a few
  ///specific methods that consolidate the process for all protocols.
  class IProtocol
  {
  protected:
    ProtocolType ptype;
    MessageList *messages; ///< The messages to send in the packet.

  public:
    ///Save a pointer to the message that will write itself into the packet.
    IProtocol( const ProtocolType &type ) : ptype(type) {;}
    ///Keep all destructors virtual.
    virtual ~IProtocol( void ) {;}

    ///Clears messages from internal message list.
    void ClearMessages( void );

    ///Add a message to send. Doesn't delete messages when done with them.
    int SetMessageList( MessageList *msgs );

    ///Formats the buffer with all the packet info it needs.
    int FormatPacket( DataStream &stream ) const;

    ///Takes the buffer of data received and extracts the message data and builds messages.
    int ExtractMessages( DataStream &stream, MessageList *messages );

  protected:
    ///Writes the header into the buffer. Header explains the type of message.
    ///Returns the number of bytes written to the buffer so as to get an offset.
    virtual void FormatHeader( DataStream &stream ) const = 0;

    ///Because Pepper > Salt : Salted hash added here if needed.
    virtual void PepperMessage( DataStream &stream ) const {;}


    ///Decides whether the packet is a valid message, takes care of decryption and unsalting.
    virtual bool ValidateMessage( DataStream &stream ) = 0;

    ///Strips the header off the message and stores any data relevant to message creation.
    ///Returns the number of bytes stripped off, so that only the message data is left.
    virtual void StripHeader( DataStream &stream ) = 0;
  
    ///Builds a message with the remaining data after the message has been decrypted/validated.
    virtual int BuildMessages( DataStream &stream, MessageList *messages ) = 0;
  };

}
