#include <yarrr/object_state_update.hpp>
#include <yarrr/object.hpp>
#include <yarrr/bitmagic.hpp>

namespace yarrr
{
ObjectStateUpdate::ObjectStateUpdate( const Object& object )
  : m_object( object )
{
}


const Object &
ObjectStateUpdate::object() const
{
  return m_object;
}


void
ObjectStateUpdate::do_serialize( Serializer& serializer ) const
{
  serializer
    .push_back( m_object.id )
    .push_back( m_object.coordinate.x )
    .push_back( m_object.coordinate.y )
    .push_back( m_object.velocity.x )
    .push_back( m_object.velocity.y )
    .push_back( m_object.angle )
    .push_back( m_object.vangle )
    .push_back( m_object.timestamp );
}


void
ObjectStateUpdate::do_deserialize( Deserializer& deserializer )
{
  m_object.id = deserializer.pop_front<uint64_t>();
  m_object.coordinate.x = deserializer.pop_front<int64_t>();
  m_object.coordinate.y = deserializer.pop_front<int64_t>();
  m_object.velocity.x = deserializer.pop_front<int64_t>();
  m_object.velocity.y = deserializer.pop_front<int64_t>();
  m_object.angle = deserializer.pop_front<int16_t>();
  m_object.vangle = deserializer.pop_front<int16_t>();
  m_object.timestamp = deserializer.pop_front<the::time::Time>();
}

}

