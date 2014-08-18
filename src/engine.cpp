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

Engine::~Engine()
{
}

void
Engine::register_to( Object& owner )
{
  m_physical_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
  m_ship_control.reset( new ShipControl( *m_physical_parameters ) );
  owner.dispatcher.register_listener< yarrr::Command  >(
      std::bind( &Engine::handle_command, this, std::placeholders::_1 ) );
  owner.dispatcher.register_listener< yarrr::TimerUpdate  >(
      std::bind( &Engine::handle_timer_update, this, std::placeholders::_1 ) );
  owner.components.register_component( *this );
}


ObjectBehavior::Pointer
Engine::clone() const
{
  return Pointer( new Engine() );
}

void
Engine::handle_command( const yarrr::Command& command )
{
  m_ship_control->handle_command( command );
  m_thruster.activate( command.timestamp() );
}

void
Engine::handle_timer_update( const yarrr::TimerUpdate& update ) const
{
  if ( !m_thruster.is_active_at( update.timestamp ) )
  {
    return;
  }

  m_particle_source.create(
      m_physical_parameters->timestamp,
      m_physical_parameters->coordinate - heading( *m_physical_parameters, 100 ),
      m_physical_parameters->velocity - heading( *m_physical_parameters, 500 ) );
}

void
Engine::do_serialize( Serializer& serializer ) const
{
}

void
Engine::do_deserialize( Deserializer& deserializer )
{
}

const the::time::Time Jet::cooldown_time{ 100000 };

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

}

