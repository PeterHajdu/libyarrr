#include <yarrr/object.hpp>
#include <yarrr/object_update.hpp>
#include <yarrr/object_behavior.hpp>
#include <yarrr/bitmagic.hpp>

#include <algorithm>

namespace
{
  yarrr::BehaviorContainer
  clone_behaviors_for_synchronization( const yarrr::BehaviorContainer& behaviors )
  {
    yarrr::BehaviorContainer cloned_container;
    for( const auto& behavior : behaviors )
    {
      if ( !behavior->should_synchronize() )
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

const int Object::object_initialization_period = 50;


Object::Object()
  : id( Id( this ) )
  , m_object_update_index( 0 )
{
}

Object::Object( const Id& id )
  : id( id )
  , m_object_update_index( 0 )
{
}

void
Object::add_behavior( ObjectBehavior::Pointer&& behavior )
{
  behavior->register_to( *this );
  m_behaviors.emplace_back( std::move( behavior ) );
}

void
Object::add_behavior_clone( ObjectBehavior& behavior )
{
  add_behavior( behavior.clone() );
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

void
Object::force_full_synchronization() const
{
  for ( auto& behavior : m_behaviors )
  {
    behavior->force_synchronization();
  }
}

ObjectUpdate::Pointer
Object::generate_update() const
{
  ObjectUpdate::Pointer update;
  if ( m_object_update_index % object_initialization_period == 0 )
  {
    force_full_synchronization();
    update.reset( new ObjectInitializer( id, clone_behaviors_for_synchronization( m_behaviors ) ) );
  }
  else
  {
    update.reset( new BasicObjectUpdate( id, clone_behaviors_for_synchronization( m_behaviors ) ) );
  }

  ++m_object_update_index;
  return update;
}

Object::Pointer
Object::clone() const
{
  Object::Pointer clone( new Object() );
  for ( const auto& behavior : m_behaviors )
  {
    clone->add_behavior( behavior->clone() );
  }
  return clone;
}

}

