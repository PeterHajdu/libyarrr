#include <yarrr/chat_message.hpp>
#include <yarrr/entity_factory.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_chat_message)
{
  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ChatMessage::ctci ), Equals( true ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::ChatMessage message( test_message, sender );
    yarrr::Data serialized_message( message.serialize() );

    yarrr::ChatMessage deserialized_message;
    deserialized_message.deserialize( serialized_message );
    AssertThat( deserialized_message.message(), Equals( test_message ) );
    AssertThat( deserialized_message.sender(), Equals( sender ) );
  }

  const std::string test_message{ "some message" };
  const std::string sender{ "Kilgore Trout" };
};

