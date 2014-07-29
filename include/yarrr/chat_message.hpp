#pragma once

#include <yarrr/event.hpp>
#include <yarrr/bitmagic.hpp>
#include <thectci/id.hpp>
#include <string>

namespace yarrr
{

class ChatMessage : public Event
{
  public:
    add_polymorphic_ctci( "yarrr_chat_message" );

    ChatMessage();

    ChatMessage( const std::string& message, const std::string& sender )
      : m_message( message )
      , m_sender( sender )
    {
    }

    const std::string& message() const
    {
      return m_message;
    }

    const std::string& sender() const
    {
      return m_sender;
    }

  private:
    std::string m_message;
    std::string m_sender;

    virtual void do_serialize( Serializer& serializer ) const override
    {
      serializer.push_back( m_message );
      serializer.push_back( m_sender );
    }

    virtual void do_deserialize( Deserializer& deserializer ) override
    {
      m_message = deserializer.pop_front< std::string >();
      m_sender = deserializer.pop_front< std::string >();
    }
};

}

