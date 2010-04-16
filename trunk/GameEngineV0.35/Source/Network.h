#pragma once

#include "NetAPI.h"
#include "System.h"

namespace Framework
{
  class Network : public ISystem
  {
    ///Maybe make separate channels for Chat and Game.
    UDPSOCKET socket; ///< Connection to server/clients.
  public:
    Network( void );
    ~Network( void );

    virtual std::string GetName( void ) { return "Network"; }

    virtual void SendMessage( Message *m );

    virtual void Update( float dt );
    virtual void Initialize( void );

  };
}
