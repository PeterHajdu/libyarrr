#include <yarrr/basic_behaviors.hpp>
#include <yarrr/command.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/graphical_engine.hpp>

#include <thectci/service_registry.hpp>

namespace
{
  yarrr::AutoEntityRegister< yarrr::PhysicalBehavior > auto_physical_behavior_register;
  yarrr::AutoEntityRegister< yarrr::Engine > auto_engine_register;
  yarrr::AutoEntityRegister< yarrr::ShipGraphics > auto_ship_graphics_register;
  yarrr::AutoEntityRegister< yarrr::LaserGraphics > auto_laser_graphics_register;
}

namespace yarrr
{


PhysicalBehavior::PhysicalBehavior( const PhysicalParameters& physical_parameters )
  : physical_parameters( physical_parameters )
{
}

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
PhysicalBehavior::do_serialize( yarrr::Serializer& serializer ) const
{
  serializer
    .push_back( physical_parameters.coordinate.x )
    .push_back( physical_parameters.coordinate.y )
    .push_back( physical_parameters.velocity.x )
    .push_back( physical_parameters.velocity.y )
    .push_back( physical_parameters.angle )
    .push_back( physical_parameters.vangle )
    .push_back( physical_parameters.timestamp );
}


void
PhysicalBehavior::do_deserialize( yarrr::Deserializer& deserializer )
{
  physical_parameters.coordinate.x = deserializer.pop_front<int64_t>();
  physical_parameters.coordinate.y = deserializer.pop_front<int64_t>();
  physical_parameters.velocity.x = deserializer.pop_front<int64_t>();
  physical_parameters.velocity.y = deserializer.pop_front<int64_t>();
  physical_parameters.angle = deserializer.pop_front<int16_t>();
  physical_parameters.vangle = deserializer.pop_front<int16_t>();
  physical_parameters.timestamp = deserializer.pop_front<the::time::Time>();
}


ObjectBehavior::Pointer
PhysicalBehavior::clone() const
{
  return ObjectBehavior::Pointer( new PhysicalBehavior( physical_parameters ) );
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
  registry.register_component( *this );
}


ObjectBehavior::Pointer
Engine::clone() const
{
  return Pointer( new Engine() );
}


Canon::Canon( ObjectContainer& object_container )
: m_object_container( object_container )
, m_physical_behavior( nullptr )
{
}


void
Canon::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& components )
{
  dispatcher.register_listener< yarrr::Command  >(
      std::bind( &Canon::handle_command, this, std::placeholders::_1 ) );
  m_physical_behavior = &components.component< yarrr::PhysicalBehavior >();
  components.register_component( *this );
}


void
Canon::handle_command( const Command& command ) const
{
  if ( command.type() != Command::fire )
  {
    return;
  }

  assert( m_physical_behavior );
  m_object_container.add_object( create_laser( m_physical_behavior->physical_parameters ) );
}


ObjectBehavior::Pointer
Canon::clone() const
{
  return Pointer( new Canon( m_object_container ) );
}

GraphicalBehavior::GraphicalBehavior()
  : yarrr::GraphicalObject( the::ctci::service< yarrr::GraphicalEngine >() )
  , m_physical_behavior( nullptr )
{
}

void
GraphicalBehavior::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& registry )
{
  m_physical_behavior = &registry.component< yarrr::PhysicalBehavior >();
  dispatcher.register_listener< FocusOnObject >( std::bind(
        &GraphicalBehavior::handle_focus_on_object, this, std::placeholders::_1 ) );
}

void
GraphicalBehavior::handle_focus_on_object( const FocusOnObject& )
{
  assert( m_physical_behavior );
  m_graphical_engine.focus_to( m_physical_behavior->physical_parameters.coordinate );
}


void
ShipGraphics::draw() const
{
  assert( m_physical_behavior );
  m_graphical_engine.draw_ship( m_physical_behavior->physical_parameters );
}

ObjectBehavior::Pointer
ShipGraphics::clone() const
{
  return Pointer( new ShipGraphics() );
}

void
ShipGraphics::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& components )
{
  components.register_component( *this );
  GraphicalBehavior::register_to( dispatcher, components );
}


void
LaserGraphics::draw() const
{
  assert( m_physical_behavior );
  m_graphical_engine.draw_laser( m_physical_behavior->physical_parameters );
}

ObjectBehavior::Pointer
LaserGraphics::clone() const
{
  return Pointer( new LaserGraphics() );
}


void
LaserGraphics::register_to(
    the::ctci::Dispatcher& dispatcher,
    the::ctci::ComponentRegistry& components )
{
  components.register_component( *this );
  GraphicalBehavior::register_to( dispatcher, components );
}

Object::Pointer
create_ship( ObjectContainer& objects )
{
  yarrr::Object::Pointer ship( new yarrr::Object() );
  ship->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );
  ship->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Engine() ) );
  ship->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::ShipGraphics() ) );
  ship->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Canon( objects ) ) );
  return ship;
}

Object::Pointer
create_laser( const PhysicalParameters& ships_parameters )
{
  yarrr::Object::Pointer ship( new yarrr::Object() );
  std::unique_ptr< PhysicalBehavior > physical_behavior( new yarrr::PhysicalBehavior( ships_parameters ) );
  physical_behavior->physical_parameters.vangle = 0;
  ship->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );
  ship->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::LaserGraphics() ) );
  return ship;
}

}

