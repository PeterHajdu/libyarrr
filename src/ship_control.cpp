#include <yarrr/ship_control.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>

namespace
{
  yarrr::AutoEntityRegister<yarrr::ShipControl> register_shipcontrol;
}

namespace yarrr
{


ShipControl::ShipControl( Type type, the::time::Time timestamp )
  : m_type( type )
  , m_timestamp( timestamp )
{
}


ShipControl::Type
ShipControl::type() const
{
  return m_type;
}


const the::time::Time&
ShipControl::timestamp() const
{
  return m_timestamp;
}


void
ShipControl::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_type );
  serializer.push_back( m_timestamp );
}


void
ShipControl::do_deserialize( Deserializer& deserializer )
{
  m_type = deserializer.pop_front<Type>();
  m_timestamp = deserializer.pop_front<the::time::Time>();
}

}

