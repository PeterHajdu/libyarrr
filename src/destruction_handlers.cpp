#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/log.hpp>
#include <thectci/service_registry.hpp>

namespace yarrr
{

DeleteWhenDestroyed::DeleteWhenDestroyed()
  : ObjectBehavior( do_not_synchronize() )
{
}

void
DeleteWhenDestroyed::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >( std::bind(
        &DeleteWhenDestroyed::handle_object_destroyed, this, std::placeholders::_1 ) );
}

ObjectBehavior::Pointer
DeleteWhenDestroyed::clone() const
{
  return Pointer( new DeleteWhenDestroyed() );
}


void
DeleteWhenDestroyed::handle_object_destroyed( const ObjectDestroyed& ) const
{
  thelog( yarrr::log::debug )( "Delete when destroyed deletes object", m_object->id() );
  the::ctci::service< EngineDispatcher >().dispatch( DeleteObject( m_object->id() ) );
}


RespawnWhenDestroyed::RespawnWhenDestroyed()
  : ObjectBehavior( do_not_synchronize() )
{
}


void
RespawnWhenDestroyed::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >( std::bind(
        &RespawnWhenDestroyed::handle_object_destroyed, this, std::placeholders::_1 ) );
}

ObjectBehavior::Pointer
RespawnWhenDestroyed::clone() const
{
  return Pointer( new RespawnWhenDestroyed() );
}


void
RespawnWhenDestroyed::handle_object_destroyed( const ObjectDestroyed& ) const
{
  the::ctci::service< EngineDispatcher >().dispatch( PlayerKilled( m_object->id() ) );
}


PlayerKilled::PlayerKilled( const Object::Id& id )
  : object_id( id )
{
}


}

