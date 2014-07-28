#include <yarrr/chat_message.hpp>
#include <yarrr/event_factory.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_chat_message)
{
  It( is_registered_to_event_factory )
  {
    AssertThat( yarrr::EventFactory::is_registered( yarrr::ChatMessage::ctci ), Equals( true ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::ChatMessage message( test_message );
    yarrr::Data serialized_message( message.serialize() );

    yarrr::ChatMessage deserialized_message;
    deserialized_message.deserialize( serialized_message );
    AssertThat( deserialized_message.message(), Equals( test_message ) );
  }

  const std::string test_message{ "some message" };
};

