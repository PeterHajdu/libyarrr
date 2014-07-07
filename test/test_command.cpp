#include <yarrr/command.hpp>
#include <yarrr/event_factory.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_command)
{
  It( is_registered_to_event_factory )
  {
    AssertThat( yarrr::EventFactory::is_registered( yarrr::Command::ctci ), Equals( true ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::Command command( command_type, timestamp );
    yarrr::Data serialized_command( command.serialize() );

    yarrr::Command deserialized_command;
    deserialized_command.deserialize( serialized_command );
    AssertThat( deserialized_command.type(), Equals( command_type ) );
    AssertThat( deserialized_command.timestamp(), Equals( timestamp ) );
  }

  const yarrr::Command::Type command_type{ 123 };
  const the::time::Time timestamp{ 246 };
};

