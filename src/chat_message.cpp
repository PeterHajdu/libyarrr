#include <yarrr/chat_message.hpp>
#include <yarrr/entity_factory.hpp>

namespace
{
  yarrr::AutoEntityRegister<yarrr::ChatMessage> register_chat_message;
}


namespace yarrr
{

ChatMessage::ChatMessage( const std::string& message, const std::string& sender )
  : m_message( message )
  , m_sender( sender )
{
}

const std::string&
ChatMessage::message() const
{
  return m_message;
}

const std::string&
ChatMessage::sender() const
{
  return m_sender;
}


void
ChatMessage::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_message );
  serializer.push_back( m_sender );
}

void
ChatMessage::do_deserialize( Deserializer& deserializer )
{
  m_message = deserializer.pop_front< std::string >();
  m_sender = deserializer.pop_front< std::string >();
}

}

