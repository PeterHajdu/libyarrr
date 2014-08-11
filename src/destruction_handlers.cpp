#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>
#include <thectci/service_registry.hpp>

namespace yarrr
{

void
DeleteWhenDestroyed::register_to( Object& owner )
{
  owner.components.register_component( *this );
  m_object_id = owner.id;
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
  the::ctci::service< EngineDispatcher >().dispatch( DeleteObject( m_object_id ) );
}

}

