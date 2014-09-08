#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <thectci/service_registry.hpp>

namespace yarrr
{

DeleteWhenDestroyed::DeleteWhenDestroyed()
  : ObjectBehavior( do_not_synchronize )
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
  the::ctci::service< EngineDispatcher >().dispatch( DeleteObject( m_object->id ) );
}


RespawnWhenDestroyed::RespawnWhenDestroyed()
  : ObjectBehavior( do_not_synchronize )
  , m_physical_parameters( nullptr )
{
}


void
RespawnWhenDestroyed::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >( std::bind(
        &RespawnWhenDestroyed::handle_object_destroyed, this, std::placeholders::_1 ) );
  assert( owner.components.has_component< yarrr::PhysicalBehavior >() );
  m_physical_parameters = &owner.components.component< yarrr::PhysicalBehavior >().physical_parameters;
}

ObjectBehavior::Pointer
RespawnWhenDestroyed::clone() const
{
  return Pointer( new RespawnWhenDestroyed() );
}


void
RespawnWhenDestroyed::handle_object_destroyed( const ObjectDestroyed& ) const
{
  assert( m_physical_parameters );
  m_physical_parameters->velocity = yarrr::Coordinate( 0, 0 );
  m_physical_parameters->coordinate = yarrr::Coordinate( 0, 0 );
}

}

