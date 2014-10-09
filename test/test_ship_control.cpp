#include <yarrr/ship_control.hpp>
#include <yarrr/entity_factory.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_command)
{
  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ShipControl::ctci ), Equals( true ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::ShipControl command( command_type, timestamp );
    yarrr::Data serialized_command( command.serialize() );

    yarrr::ShipControl deserialized_command;
    deserialized_command.deserialize( serialized_command );
    AssertThat( deserialized_command.type(), Equals( command_type ) );
    AssertThat( deserialized_command.timestamp(), Equals( timestamp ) );
  }

  const yarrr::ShipControl::Type command_type{ 123 };
  const the::time::Time timestamp{ 246 };
};

