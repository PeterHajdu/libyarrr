#include <yarrr/character.hpp>
#include <yarrr/object.hpp>

namespace yarrr
{

Character::Character( the::model::Node& parent )
  : m_character_model( "character",  parent )
  , m_object_id( "object_id", "", m_character_model )
{
}


void
Character::assign_object( const Object& object )
{
  m_object_id = std::to_string( object.id() );
}

}

