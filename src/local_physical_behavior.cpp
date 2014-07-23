#include <yarrr/local_physical_behavior.hpp>

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

}

