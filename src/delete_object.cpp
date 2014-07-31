#include <yarrr/delete_object.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>

namespace
{
  yarrr::AutoEntityRegister< yarrr::DeleteObject > register_command;
}

namespace yarrr
{

DeleteObject::DeleteObject( const Object::Id& id )
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
  m_object_id = deserializer.pop_front<Object::Id>();
}

const Object::Id&
DeleteObject::object_id() const
{
  return m_object_id;
}

}

