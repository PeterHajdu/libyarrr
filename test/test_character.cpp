#include <yarrr/character.hpp>
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/lua_engine.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_character)
{
  void SetUp()
  {
    lua.reset( new sol::state() );
    lua->open_libraries( sol::lib::base );

    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );
    yarrr::component_of< yarrr::PhysicalBehavior >( *object ).physical_parameters.coordinate = object_coordinate;

    character.reset( new yarrr::Character( *lua ) );
    character->assign_object( *object );

    character_table = &character->table();
    ( *lua )[ "the_character" ] = character->table();
  }


  It( exports_its_object_id )
  {
    AssertThat( yarrr::lua_script( *lua, std::string( "assert( the_character.object_id == \"" ) + std::to_string( object->id() ) + "\" )" ), Equals( true ) );
  }

  const yarrr::Coordinate object_coordinate{ 1234_metres, -29843_metres };
  yarrr::Object::Pointer object;
  yarrr::Character::Pointer character;
  std::unique_ptr< sol::state > lua;
  const sol::table* character_table;
};

