#pragma once

#include "INetMessage.h"

namespace Framework
{
  class ChatMessage : public INetMessage
  {
    enum MsgType
    {
       CM_Invalid
      ,CM_Data_Text
      ,CM_Data_Address

    };
  public:
  };
}
