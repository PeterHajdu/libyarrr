#include <yarrr/character.hpp>
#include <yarrr/lua_engine.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe_Only(a_character)
{
  void SetUp()
  {
    character.reset( new yarrr::Character() );
  }

  yarrr::Character::Pointer character;
};

