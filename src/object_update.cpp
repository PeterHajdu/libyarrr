#include <yarrr/object_update.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/log.hpp>

namespace
{
  yarrr::AutoEntityRegister<yarrr::BasicObjectUpdate> register_object_update;
  yarrr::AutoEntityRegister<yarrr::ObjectInitializer> register_object_initializer;
}

namespace yarrr
{

ObjectUpdate::ObjectUpdate()
  : m_id( 0 )
{
}

ObjectUpdate::ObjectUpdate( const Object::Id& id, BehaviorContainer&& behaviors )
  : m_id( id )
  , m_behaviors( std::move( behaviors ) )
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
  thelog( yarrr::log::debug )( "Serializing update. id:", id(), "number of behaviors:", m_behaviors.size() );
  serializer.push_back( m_id );
  for ( const auto& behavior : m_behaviors )
  {
    behavior->serialize( serializer );
  }
}

void
ObjectUpdate::do_deserialize( Deserializer& deserializer )
{
  m_id = deserializer.pop_front< Object::Id >();

  Entity::Pointer new_entity;
  while ( deserializer.bytes_left() && ( new_entity = EntityFactory::create( deserializer ) ) )
  {
    m_behaviors.push_back( ObjectBehavior::Pointer(
          static_cast< ObjectBehavior* >( new_entity.release() ) ) );
  }

  thelog( yarrr::log::debug )( "Deserialized update. id:", id(), "number of behaviors:", m_behaviors.size() );
}

void
ObjectUpdate::update_object( Object& object ) const
{
  thelog( yarrr::log::debug )( "Update object from update. id:", id(), "number of behaviors:", m_behaviors.size() );
  for ( const auto& behavior : m_behaviors )
  {
    object.update_behavior( behavior->clone() );
  }
}

Object::Pointer
ObjectUpdate::create_object() const
{
  thelog( yarrr::log::debug )( "Create object from update. id:", id(), "number of behaviors:", m_behaviors.size() );
  Object::Pointer recreated_object( new Object( m_id ) );
  for ( const auto& behavior : m_behaviors )
  {
    recreated_object->add_behavior( behavior->clone() );
  }
  return recreated_object;
}

BasicObjectUpdate::BasicObjectUpdate( const Object::Id& id, BehaviorContainer&& behaviors )
  : ObjectUpdate( id, std::move( behaviors ) )
{
}

ObjectInitializer::ObjectInitializer( const Object::Id& id, BehaviorContainer&& behaviors )
  : ObjectUpdate( id, std::move( behaviors ) )
{
}

}

