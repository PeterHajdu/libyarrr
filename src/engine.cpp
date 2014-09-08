#include <yarrr/engine.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/command.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/particle.hpp>
#include <yarrr/bitmagic.hpp>

#include <thectci/service_registry.hpp>

namespace
{
  const size_t particle_speed_deviation{ 50 };
}

namespace yarrr
{

Engine::Engine()
  : ObjectBehavior( synchronize )
  , m_physical_parameters( nullptr )
  , m_particle_source( particle_speed_deviation )
{
}

Engine::Engine( const Engine& other )
  : ObjectBehavior( synchronize )
  , m_physical_parameters( nullptr )
  , m_particle_source( particle_speed_deviation )
  , m_thruster( other.m_thruster )
  , m_cw_jet( other.m_cw_jet )
  , m_ccw_jet( other.m_ccw_jet )
{
}

Engine::~Engine()
{
}

void
Engine::do_register_to( Object& owner )
{
  m_physical_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
  m_ship_control.reset( new ShipControl( *m_physical_parameters ) );
  owner.dispatcher.register_listener< yarrr::Command  >(
      std::bind( &Engine::handle_command, this, std::placeholders::_1 ) );
  owner.dispatcher.register_listener< yarrr::TimerUpdate  >(
      std::bind( &Engine::handle_timer_update, this, std::placeholders::_1 ) );
  owner.dispatcher.register_listener< yarrr::Engine  >(
      std::bind( &Engine::handle_engine_update, this, std::placeholders::_1 ) );
}


ObjectBehavior::Pointer
Engine::clone() const
{
  return Pointer( new Engine( *this ) );
}


void
Engine::handle_engine_update( const yarrr::Engine& other )
{
  m_thruster = other.m_thruster;
  m_cw_jet = other.m_cw_jet;
  m_ccw_jet = other.m_ccw_jet;
}


void
Engine::handle_command( const yarrr::Command& command )
{
  m_ship_control->handle_command( command );
  switch ( command.type() )
  {
    case Command::thruster: m_thruster.activate( command.timestamp() ); break;
    case Command::cw: m_cw_jet.activate( command.timestamp() ); break;
    case Command::ccw: m_ccw_jet.activate( command.timestamp() ); break;
  }
}

void
Engine::handle_timer_update( const yarrr::TimerUpdate& update ) const
{
  const Coordinate parallel( heading( *m_physical_parameters, 70 ) );

  if ( m_thruster.is_active_at( update.timestamp ) )
  {
    m_particle_source.create(
        m_physical_parameters->timestamp,
        m_physical_parameters->coordinate - parallel,
        m_physical_parameters->velocity - parallel * 5 );
  }

  const Coordinate perp( perpendicular( parallel ) );
  if ( m_cw_jet.is_active_at( update.timestamp ) )
  {
    m_particle_source.create(
        m_physical_parameters->timestamp,
        m_physical_parameters->coordinate - parallel,
        m_physical_parameters->velocity - perp * 5 );

    m_particle_source.create(
        m_physical_parameters->timestamp,
        m_physical_parameters->coordinate + parallel,
        m_physical_parameters->velocity + perp * 5 );
  }

  if ( m_ccw_jet.is_active_at( update.timestamp ) )
  {
    m_particle_source.create(
        m_physical_parameters->timestamp,
        m_physical_parameters->coordinate - parallel,
        m_physical_parameters->velocity + perp * 5 );

    m_particle_source.create(
        m_physical_parameters->timestamp,
        m_physical_parameters->coordinate + parallel,
        m_physical_parameters->velocity - perp * 5 );
  }
}

void
Engine::do_serialize( Serializer& serializer ) const
{
  m_thruster.serialize( serializer );
  m_cw_jet.serialize( serializer );
  m_ccw_jet.serialize( serializer );
}

void
Engine::do_deserialize( Deserializer& deserializer )
{
  m_thruster.deserialize( deserializer );
  m_cw_jet.deserialize( deserializer );
  m_ccw_jet.deserialize( deserializer );
}

const the::time::Time Jet::cooldown_time{ 300000 };

Jet::Jet( const Jet& other )
  : m_active_until( other.m_active_until )
{
}

Jet::Jet()
  : m_active_until( 0 )
{
}

void
Jet::activate( const the::time::Time& at )
{
  m_active_until = at + cooldown_time;
}

bool
Jet::is_active_at( const the::time::Time& at ) const
{
  return m_active_until > at;
}

void
Jet::serialize( Serializer& serializer ) const
{
  serializer.push_back( m_active_until );
}

void
Jet::deserialize( Deserializer& deserializer )
{
  m_active_until = deserializer.pop_front< the::time::Time >();
}

Jet&
Jet::operator=( const Jet& other )
{
  m_active_until = other.m_active_until;
  return *this;
}

}

