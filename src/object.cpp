#include <yarrr/object.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>

namespace
{
  yarrr::AutoEntityRegister<yarrr::ObjectUpdate> register_object_update;
}

namespace yarrr
{

Object::Object()
  : id( Id( this ) )
{
}

Object::Object( const Id& id )
  : id( id )
{
}

void
Object::add_behavior( ObjectBehavior::Pointer&& behavior )
{
  behavior->register_to( *this, m_components );
  m_behaviors.emplace_back( std::move( behavior ) );
}

Entity::Pointer
Object::generate_update() const
{
  return Entity::Pointer( new ObjectUpdate( id ) );
}

ObjectUpdate::ObjectUpdate()
  : m_id( 0 )
{
}

ObjectUpdate::ObjectUpdate( const Object::Id& id )
  : m_id( id )
{
}

const Object::Id&
ObjectUpdate::id() const
{
  return m_id;
}

void
ObjectUpdate::do_serialize( Serializer& serializer ) const
{
}

void
ObjectUpdate::do_deserialize( Deserializer& deserializer )
{
}


}

