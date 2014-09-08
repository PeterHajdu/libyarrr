#include <yarrr/object.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>

#include <algorithm>

namespace
{
  yarrr::AutoEntityRegister<yarrr::ObjectUpdate> register_object_update;

  yarrr::BehaviorContainer
  clone_behaviors_for_synchronization( const yarrr::BehaviorContainer& behaviors )
  {
    yarrr::BehaviorContainer cloned_container;
    for( const auto& behavior : behaviors )
    {
      if ( !behavior->should_synchronize )
      {
        continue;
      }

      cloned_container.emplace_back( behavior->clone() );
    }
    return cloned_container;
  }

}

namespace yarrr
{

ObjectBehavior::ObjectBehavior( ShouldSynchronize should_synchronize )
  : should_synchronize( should_synchronize )
  , m_id( Id( this ) )
  , m_object( nullptr )
{
}

ObjectBehavior::ObjectBehavior( ShouldSynchronize should_synchronize, const Id& id )
  : should_synchronize( should_synchronize )
  , m_id( id )
  , m_object( nullptr )
{
}

const ObjectBehavior::Id&
ObjectBehavior::id() const
{
  return m_id;
}

void
ObjectBehavior::register_to( Object& owner )
{
  m_object = &owner;
  do_register_to( owner );
  owner.components.register_polymorphic_component( *this );
}

void
ObjectBehavior::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_id );
  serialize_behavior( serializer );
}

void
ObjectBehavior::do_deserialize( Deserializer& deserializer )
{
  m_id = deserializer.pop_front< Id >();
  deserialize_behavior( deserializer );
}

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
  behavior->register_to( *this );
  m_behaviors.emplace_back( std::move( behavior ) );
}


void
Object::update_behavior( ObjectBehavior::Pointer&& updater_behavior )
{
  const ObjectBehavior::Id behavior_id( updater_behavior->id() );
  BehaviorContainer::const_iterator found_behavior( std::find_if(
        std::begin( m_behaviors ), std::end( m_behaviors ),
        [ behavior_id ]( const ObjectBehavior::Pointer& behavior )
        {
          return behavior_id == behavior->id();
        }) );

  if ( found_behavior == m_behaviors.end() )
  {
    add_behavior( std::move( updater_behavior ) );
    return;
  }

  (*found_behavior)->update( *updater_behavior );
}


ObjectUpdate::Pointer
Object::generate_update() const
{
  return ObjectUpdate::Pointer( new ObjectUpdate( id, clone_behaviors_for_synchronization( m_behaviors ) ) );
}

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
}

Object::Pointer
ObjectUpdate::create_object() const
{
  Object::Pointer recreated_object( new Object( m_id ) );
  for ( const auto& behavior : m_behaviors )
  {
    recreated_object->add_behavior( behavior->clone() );
  }
  return recreated_object;
}

void
ObjectUpdate::update_object( Object& object ) const
{
  for ( const auto& behavior : m_behaviors )
  {
    object.update_behavior( behavior->clone() );
  }
}

}

