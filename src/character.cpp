#include <yarrr/character.hpp>
#include <yarrr/object.hpp>

namespace yarrr
{

Character::Character( sol::state& lua )
  : m_table( lua.create_table() )
{
}


void
Character::assign_object( const Object& object )
{
  m_object_id = std::to_string( object.id() );
  m_table[ "object_id" ] = m_object_id;
}


sol::table&
Character::table()
{
  return m_table;
}


}

