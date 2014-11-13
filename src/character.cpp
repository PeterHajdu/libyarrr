#include <yarrr/character.hpp>

namespace yarrr
{

Character::Character( const std::string& object_id, the::model::OwningNode& parent )
  : m_character_model( "character",  parent )
  , m_object_id( "object_id", std::string( object_id ), m_character_model )
{
}

}

