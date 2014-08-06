#include <yarrr/basic_behaviors.hpp>
#include <yarrr/command.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/object_container.hpp>

namespace yarrr
{

void
PhysicalBehavior::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  registry.register_component< PhysicalBehavior >( *this );
  dispatcher.register_listener< TimerUpdate >( std::bind(
        &PhysicalBehavior::handle_timer_update, this, std::placeholders::_1 ) );
  dispatcher.register_listener< yarrr::PhysicalBehavior  >( std::bind(
        &PhysicalBehavior::handle_network_update, this, std::placeholders::_1 ) );
}


void
PhysicalBehavior::handle_timer_update( const TimerUpdate& timer_update )
{
  travel_in_time_to( timer_update.timestamp, physical_parameters );
}


void
PhysicalBehavior::handle_network_update( const PhysicalBehavior& update )
{
  PhysicalParameters network_parameters( update.physical_parameters );
  //todo: move to physical parameters to a function with a ratio parameter
  travel_in_time_to( physical_parameters.timestamp, network_parameters );
  physical_parameters.coordinate = ( network_parameters.coordinate + physical_parameters.coordinate ) * 0.5;
  physical_parameters.velocity = ( network_parameters.velocity + physical_parameters.velocity ) * 0.5;
  physical_parameters.angle = ( network_parameters.angle + physical_parameters.angle ) * 0.5;
  physical_parameters.vangle = ( network_parameters.vangle + physical_parameters.vangle ) * 0.5;
}

void
Engine::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  m_ship_control.reset(
      new ShipControl( registry.component< PhysicalBehavior >().physical_parameters ) );
  dispatcher.register_listener< yarrr::Command  >(
      std::bind( &ShipControl::handle_command, *m_ship_control, std::placeholders::_1 ) );
}


Canon::Canon( ObjectContainer& object_container )
  : m_object_container( object_container )
{
}


void
Canon::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& )
{
  dispatcher.register_listener< yarrr::Command  >(
      std::bind( &Canon::handle_command, this, std::placeholders::_1 ) );
}


void
Canon::handle_command( const Command& command ) const
{
  if ( command.type() != Command::fire )
  {
    return;
  }

  Object::Pointer object( new Object() );
  m_object_container.add_object( std::move( object ) );
}

}

