#include <yarrr/command.hpp>
#include <yarrr/entity_factory.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_command)
{
  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::Command::ctci ), Equals( true ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::Command command( { "command", "parameter1", "parameter2" } );
    yarrr::Data serialized_command( command.serialize() );

    yarrr::Command deserialized_command;
    deserialized_command.deserialize( serialized_command );

    AssertThat( deserialized_command.command(), Equals( "command" ) );
    AssertThat(
        deserialized_command.parameters(),
        Equals( yarrr::Command::StringTokens{ "parameter1", "parameter2" } ) );
  }

  It( works_with_one_parameter )
  {
    yarrr::Command command( { "command", "parameter1" } );
    AssertThat( command.command(), Equals( "command" ) );
    AssertThat( command.parameters(), Equals( yarrr::Command::StringTokens{ "parameter1" } ) );
  }

  It( works_without_parameter )
  {
    yarrr::Command command( { "command" } );
    AssertThat( command.command(), Equals( "command" ) );
    AssertThat( command.parameters(), Equals( yarrr::Command::StringTokens{} ) );
  }

  It( works_with_empty_token_list )
  {
    yarrr::Command command( yarrr::Command::StringTokens{} );
    AssertThat( command.command(), Equals( "" ) );
    AssertThat( command.parameters(), Equals( yarrr::Command::StringTokens{} ) );
  }

};

