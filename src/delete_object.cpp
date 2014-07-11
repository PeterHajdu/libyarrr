#include <yarrr/delete_object.hpp>
#include <yarrr/event_factory.hpp>
#include <yarrr/bitmagic.hpp>

namespace
{
  yarrr::AutoEventRegister< yarrr::DeleteObject > register_command;
}

namespace yarrr
{

DeleteObject::DeleteObject( const PhysicalParameters::Id& id )
  : m_object_id( id )
{
}

void
DeleteObject::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_object_id );
}

void
DeleteObject::do_deserialize( Deserializer& deserializer )
{
  m_object_id = deserializer.pop_front<PhysicalParameters::Id>();
}

const PhysicalParameters::Id&
DeleteObject::object_id() const
{
  return m_object_id;
}

}

