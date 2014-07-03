#include <yarrr/command.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_command)
{
  It( is_serializable_and_deserializable )
  {
    yarrr::Command command( command_id, timestamp );
    yarrr::Data serialized_command( command.serialize() );

    yarrr::Command deserialized_command;
    deserialized_command.deserialize( serialized_command );
    AssertThat( deserialized_command.id(), Equals( command_id ) );
    AssertThat( deserialized_command.timestamp(), Equals( timestamp ) );
  }

  const yarrr::Command::Id command_id{ 123 };
  const the::time::Time timestamp{ 246 };
};

