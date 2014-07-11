#include <yarrr/object_state_update.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/event_factory.hpp>

namespace
{
  yarrr::AutoEventRegister<yarrr::ObjectStateUpdate> register_object_state_update;
}

namespace yarrr
{
ObjectStateUpdate::ObjectStateUpdate( const PhysicalParameters& physical_parameters )
  : m_physical_parameters( physical_parameters )
{
}


const PhysicalParameters &
ObjectStateUpdate::physical_parameters() const
{
  return m_physical_parameters;
}


void
ObjectStateUpdate::do_serialize( Serializer& serializer ) const
{
  serializer
    .push_back( m_physical_parameters.id )
    .push_back( m_physical_parameters.coordinate.x )
    .push_back( m_physical_parameters.coordinate.y )
    .push_back( m_physical_parameters.velocity.x )
    .push_back( m_physical_parameters.velocity.y )
    .push_back( m_physical_parameters.angle )
    .push_back( m_physical_parameters.vangle )
    .push_back( m_physical_parameters.timestamp );
}


void
ObjectStateUpdate::do_deserialize( Deserializer& deserializer )
{
  m_physical_parameters.id = deserializer.pop_front<uint64_t>();
  m_physical_parameters.coordinate.x = deserializer.pop_front<int64_t>();
  m_physical_parameters.coordinate.y = deserializer.pop_front<int64_t>();
  m_physical_parameters.velocity.x = deserializer.pop_front<int64_t>();
  m_physical_parameters.velocity.y = deserializer.pop_front<int64_t>();
  m_physical_parameters.angle = deserializer.pop_front<int16_t>();
  m_physical_parameters.vangle = deserializer.pop_front<int16_t>();
  m_physical_parameters.timestamp = deserializer.pop_front<the::time::Time>();
}

}

