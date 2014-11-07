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
    lua.reset( new the::model::Lua() );
    root.reset( new the::model::Node( "root", *lua ) );
    character.reset( new yarrr::Character( *root ) );
    character->assign_object( object );
  }

  void TearDown()
  {
    character.reset();
    root.reset();
    lua.reset();
  }

  It( exports_its_object_id )
  {
    const std::string object_id_path( the::model::path_from( { "root", "character", "object_id" } ) );
    AssertThat( lua->assert_equals( object_id_path, expected_object_id ), Equals( true ) );
  }

  yarrr::Object object;
  const std::string expected_object_id{ std::to_string( object.id() ) };
  yarrr::Character::Pointer character;
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::Node > root;
};

