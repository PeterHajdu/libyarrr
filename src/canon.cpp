#include <yarrr/canon.hpp>
#include <yarrr/command.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/inventory.hpp>
#include <thectci/service_registry.hpp>
#include <yarrr/engine_dispatcher.hpp>

namespace yarrr
{

Canon::Canon()
: ObjectBehavior( do_not_syncronize )
, m_physical_parameters( nullptr )
{
}


void
Canon::register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::Command  >(
      std::bind( &Canon::handle_command, this, std::placeholders::_1 ) );
  m_physical_parameters = &owner.components.component< yarrr::PhysicalBehavior >().physical_parameters;
  owner.components.register_component( *this );
  assert( owner.components.has_component< Inventory >() );
  owner.components.component< Inventory >().register_item( *this );
}


void
Canon::handle_command( const Command& command ) const
{
  if ( command.type() != Command::fire )
  {
    return;
  }

  assert( m_physical_parameters );

  the::ctci::service< yarrr::EngineDispatcher >().dispatch( ObjectCreated(
        create_laser( *m_physical_parameters ) ) );
}


ObjectBehavior::Pointer
Canon::clone() const
{
  return Pointer( new Canon() );
}

}

