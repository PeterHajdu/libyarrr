#pragma once

#include <thectci/id.hpp>

namespace yarrr
{

class ChatMessage
{
  public:
    add_ctci( "yarrr_chat_message" );

    ChatMessage( const std::string& message )
      : message( message )
    {
    }

    const std::string message;
};

}

