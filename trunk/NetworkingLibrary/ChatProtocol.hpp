#ifndef CHAT_PROTOCOL_H
#define CHAT_PROTOCOL_H

#include "NetAPI.h"

struct DirectedMessage
{
  DirectedMessage( const std::string &to, const std::string &from )
  { strcpy(to_,to.c_str()); strcpy(from_,from.c_str()); }

  char to_[32];   ///< Who the message is to.
  char from_[32]; ///< Who the message is from.
};

typedef unsigned TransferID;

enum MessageType { MT_SEND_FILE, MT_ACCEPT_FILE, MT_REJECT_FILE, MT_INVALID_MSG };

struct FileTransferInfo
{
  FileTransferInfo(TransferID id, MessageType type, const std::string &to,
    const std::string &from, const std::string &file, const NAPI::NetAddress &adr)
    : id_(id), type_(type), udp_(adr)
  { strcpy(to_, to.c_str()); strcpy(from_,from.c_str()); strcpy(file_,file.c_str()); }

  char to_[32];          ///< Username to send to.
  char from_[32];        ///< User sending the file.
  MessageType type_;      ///< The type of message related to the transfer.
  TransferID id_;        ///< An ID to keep track of which transfer
  char file_[MAX_PATH];  ///< Filename to send.
  NAPI::NetAddress udp_; ///< The UDP socket to communicate with.
};

#endif
