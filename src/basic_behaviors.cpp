#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object_state_update.hpp>
#include <yarrr/command.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/object_container.hpp>

namespace yarrr
{

void
LocalPhysicalBehavior::register_to(
    the::ctci::Dispatcher&,
    the::ctci::ComponentRegistry& registry )
{
  registry.register_component< LocalPhysicalBehavior >( *this );
}


SimplePhysicsUpdater::SimplePhysicsUpdater()
  : m_local_physical_behavior( nullptr )
{
}


void
SimplePhysicsUpdater::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  m_local_physical_behavior = &registry.component< LocalPhysicalBehavior >();
  dispatcher.register_listener< TimerUpdate >( std::bind(
        &SimplePhysicsUpdater::handle_timer_update, this, std::placeholders::_1 ) );
}

void
SimplePhysicsUpdater::handle_timer_update( const TimerUpdate& timer_update ) const
{
  assert( m_local_physical_behavior );
  travel_in_time_to( timer_update.timestamp, m_local_physical_behavior->physical_parameters );
}


NetworkSynchronizer::NetworkSynchronizer()
  : m_local_physical_behavior( nullptr )
{
}


void
NetworkSynchronizer::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  m_local_physical_behavior = &registry.component< LocalPhysicalBehavior >();
  dispatcher.register_listener< yarrr::ObjectStateUpdate  >( std::bind(
        &NetworkSynchronizer::handle_object_state_update, this, std::placeholders::_1 ) );
}

void
NetworkSynchronizer::handle_object_state_update( const ObjectStateUpdate& object_state_update ) const
{
  PhysicalParameters network_parameters( object_state_update.physical_parameters() );
  assert( m_local_physical_behavior );
  //todo: move to physical parameters to a function with a ratio parameter
  PhysicalParameters& local_parameters( m_local_physical_behavior->physical_parameters );
  travel_in_time_to( local_parameters.timestamp, network_parameters );
  local_parameters.coordinate = ( network_parameters.coordinate + local_parameters.coordinate ) * 0.5;
  local_parameters.velocity = ( network_parameters.velocity + local_parameters.velocity ) * 0.5;
  local_parameters.angle = ( network_parameters.angle + local_parameters.angle ) * 0.5;
  local_parameters.vangle = ( network_parameters.vangle + local_parameters.vangle ) * 0.5;
}

void
Engine::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  m_ship_control.reset(
      new ShipControl( registry.component< LocalPhysicalBehavior >().physical_parameters ) );
  dispatcher.register_listener< yarrr::Command  >(
      std::bind( &ShipControl::handle_command, *m_ship_control, std::placeholders::_1 ) );
}

PhysicalParameterSerializer::PhysicalParameterSerializer()
  : m_local_physical_behavior( nullptr )
{
}


void
PhysicalParameterSerializer::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  m_local_physical_behavior = &registry.component< LocalPhysicalBehavior >();
  dispatcher.register_listener< yarrr::SerializePhysicalParameter  >( std::bind(
        &PhysicalParameterSerializer::handle_serialize, this, std::placeholders::_1 ) );
}

void
PhysicalParameterSerializer::handle_serialize( const SerializePhysicalParameter& serialize ) const
{
  assert( m_local_physical_behavior );
  serialize.data_buffer.push_back(
      ObjectStateUpdate( m_local_physical_behavior->physical_parameters ).serialize() );
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
  m_object_container.add_object( Object::Id( object.get() ), std::move( object ) );
}

}

