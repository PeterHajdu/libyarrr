#include <yarrr/character.hpp>
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
    character.reset( new yarrr::Character( object_id, *root ) );
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
    AssertThat( lua->assert_equals( object_id_path, object_id ), Equals( true ) );
  }

  const std::string object_id{ "384658" };
  yarrr::Character::Pointer character;
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::Node > root;
};

