#include <yarrr/basic_behaviors.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/graphical_engine.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/thruster.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/log.hpp>

#include <thectci/service_registry.hpp>

#include <cassert>

namespace
{
  yarrr::AutoEntityRegister< yarrr::PhysicalBehavior > auto_physical_behavior_register;
  yarrr::AutoEntityRegister< yarrr::LaserGraphics > auto_laser_graphics_register;
  yarrr::AutoEntityRegister< yarrr::ShapeGraphics > auto_shape_graphics_register;

  yarrr::PhysicalParameters
  zero_parameters()
  {
    yarrr::PhysicalParameters parameters;
    parameters.coordinate = { 0, 0 };
    parameters.velocity = { 0, 0 };
    parameters.orientation = 0;
    parameters.angular_velocity = 0;
    parameters.timestamp = 0;
    return parameters;
  }
}

namespace yarrr
{

Coordinate& coordinate_of( const Object& object )
{
  assert( yarrr::has_component< yarrr::PhysicalBehavior >( object ) );
  return yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.coordinate;
}

Velocity& velocity_of( const Object& object )
{
  assert( yarrr::has_component< yarrr::PhysicalBehavior >( object ) );
  return yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.velocity;
}

PhysicalBehavior::PhysicalBehavior()
  : ObjectBehavior( always_synchronize() )
  , physical_parameters( zero_parameters() )
{
}

PhysicalBehavior::PhysicalBehavior( const PhysicalParameters& physical_parameters )
  : ObjectBehavior( always_synchronize() )
  , physical_parameters( physical_parameters )
{
}

PhysicalBehavior::PhysicalBehavior( const PhysicalParameters& physical_parameters, const Id& id )
  : ObjectBehavior( always_synchronize(), id )
  , physical_parameters( physical_parameters )
{
}

void
PhysicalBehavior::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< TimerUpdate >( std::bind(
        &PhysicalBehavior::handle_timer_update, this, std::placeholders::_1 ) );
}


void
PhysicalBehavior::handle_timer_update( const TimerUpdate& timer_update )
{
  travel_in_time_to( timer_update.timestamp, physical_parameters );
}


void
PhysicalBehavior::update( const ObjectBehavior& behavior )
{
  const PhysicalBehavior& update( static_cast< const PhysicalBehavior& >( behavior ) );
  PhysicalParameters network_parameters( update.physical_parameters );
  travel_in_time_to( physical_parameters.timestamp, network_parameters );
  physical_parameters.integrity = network_parameters.integrity;
  physical_parameters = weight_arithmetic_mean( physical_parameters, network_parameters, 0.9 );
}

void
PhysicalBehavior::serialize_behavior( yarrr::Serializer& serializer ) const
{
  serializer
    .push_back( physical_parameters.coordinate.x )
    .push_back( physical_parameters.coordinate.y )
    .push_back( physical_parameters.velocity.x )
    .push_back( physical_parameters.velocity.y )
    .push_back( physical_parameters.orientation )
    .push_back( physical_parameters.angular_velocity )
    .push_back( physical_parameters.integrity )
    .push_back( physical_parameters.timestamp );
}


void
PhysicalBehavior::deserialize_behavior( yarrr::Deserializer& deserializer )
{
  physical_parameters.coordinate.x = deserializer.pop_front< Coordinate::type >();
  physical_parameters.coordinate.y = deserializer.pop_front< Coordinate::type >();
  physical_parameters.velocity.x = deserializer.pop_front< Coordinate::type >();
  physical_parameters.velocity.y = deserializer.pop_front< Coordinate::type >();
  physical_parameters.orientation = deserializer.pop_front< Angle >();
  physical_parameters.angular_velocity = deserializer.pop_front< Angle >();
  physical_parameters.integrity = deserializer.pop_front<int16_t>();
  physical_parameters.timestamp = deserializer.pop_front<the::time::Time>();
}


ObjectBehavior::Pointer
PhysicalBehavior::clone() const
{
  return ObjectBehavior::Pointer( new PhysicalBehavior( physical_parameters, m_id ) );
}

SelfDestructor::SelfDestructor(
    Object::Id object_id,
    const the::time::Time& lifespan )
: ObjectBehavior( do_not_synchronize() )
, m_lifespan( lifespan )
, m_object_id( object_id )
, m_time_to_die( 0u )
{
}


void
SelfDestructor::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::TimerUpdate  >(
      std::bind( &SelfDestructor::handle_timer_update, this, std::placeholders::_1 ) );
}


void
SelfDestructor::handle_timer_update( const TimerUpdate& timer_update )
{
  if ( !m_time_to_die )
  {
    m_time_to_die = timer_update.timestamp + m_lifespan;
    return;
  }

  if ( m_time_to_die > timer_update.timestamp )
  {
    return;
  }

  thelog( yarrr::log::debug )( "Self destructor deletes object", m_object_id );
  the::ctci::service< yarrr::EngineDispatcher >().dispatch( DeleteObject( m_object_id ) );
}


ObjectBehavior::Pointer
SelfDestructor::clone() const
{
  return Pointer( new SelfDestructor( m_object_id, m_lifespan ) );
}


GraphicalBehavior::GraphicalBehavior()
  : ObjectBehavior( rarely_synchronize() )
  , yarrr::GraphicalObject( the::ctci::service< yarrr::GraphicalEngine >() )
  , m_physical_behavior( nullptr )
{
}

GraphicalBehavior::GraphicalBehavior( const Id& id )
  : ObjectBehavior( rarely_synchronize(), id )
  , yarrr::GraphicalObject( the::ctci::service< yarrr::GraphicalEngine >() )
  , m_physical_behavior( nullptr )
{
}

void
GraphicalBehavior::do_register_to( Object& owner )
{
  m_physical_behavior = &owner.components.component< yarrr::PhysicalBehavior >();
  owner.dispatcher.register_listener< FocusOnObject >( std::bind(
        &GraphicalBehavior::handle_focus_on_object, this, std::placeholders::_1 ) );
  register_graphical_behavior_to( owner );
}

void
GraphicalBehavior::handle_focus_on_object( const FocusOnObject& )
{
  assert( m_physical_behavior );
  m_graphical_engine.focus_to( *m_object );
}

LaserGraphics::LaserGraphics( const Id& id )
  : GraphicalBehavior( id )
{
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
  return Pointer( new LaserGraphics( id() ) );
}

ShapeGraphics::ShapeGraphics( const Id& id )
  : GraphicalBehavior( id )
{
}

//todo: show/hide on graphical objects might be a better solution
void
ShapeGraphics::draw() const
{
  if ( !m_physical_behavior )
  {
    return;
  }

  m_graphical_engine.draw_object_with_shape( *m_object );
}

void
ShapeGraphics::register_graphical_behavior_to( Object& owner )
{
  assert( owner.components.has_component< ShapeBehavior >() );
}

ObjectBehavior::Pointer
ShapeGraphics::clone() const
{
  return Pointer( new ShapeGraphics( id() ) );
}
}

