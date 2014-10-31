#include <yarrr/thruster.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/particle.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/types.hpp>
#include <yarrr/coordinate_transformation.hpp>
#include <yarrr/log.hpp>

#include <thectci/service_registry.hpp>

namespace
{
  const size_t particle_speed_deviation{ 6_metres };
  yarrr::AutoEntityRegister< yarrr::Thruster > auto_thruster_register;

  const char * thruster_name = "thruster";

  double strength_of_pushing_force(
      yarrr::Coordinate normalized_relative_coordinate,
      const yarrr::Coordinate& relative_force_vector,
      int mass_and_engine_power_ratio )
  {
    if ( normalized_relative_coordinate == yarrr::Coordinate{ 0, 0 } )
    {
      normalized_relative_coordinate = relative_force_vector * -1;
    }

    return -1.0 * dot_product( normalized_relative_coordinate, relative_force_vector ) / mass_and_engine_power_ratio;
  }

}

namespace yarrr
{

Thruster::Thruster()
  : Item( rarely_synchronize(), thruster_name, { 0, 0 } )
  , m_physical_parameters( nullptr )
  , m_shape( nullptr )
  , m_particle_source( particle_speed_deviation )
  , m_activation_command( ShipControl::main_thruster )
{
}

Thruster::Thruster(
    ShipControl::Type activation_command,
    const Tile::Coordinate& coordinate,
    Angle direction )
  : Item( rarely_synchronize(), thruster_name, coordinate )
  , m_physical_parameters( nullptr )
  , m_shape( nullptr )
  , m_particle_source( particle_speed_deviation )
  , m_activation_command( activation_command )
  , m_normalized_relative_coordinate()
  , m_direction( direction )
{
}

Thruster::Thruster( const Thruster& other )
  : Item( rarely_synchronize(), other.m_id, thruster_name, other.tile_coordinate() )
  , m_physical_parameters( nullptr )
  , m_shape( nullptr )
  , m_particle_source( particle_speed_deviation )
  , m_jet( other.m_jet )
  , m_activation_command( other.m_activation_command )
  , m_normalized_relative_coordinate( other.m_normalized_relative_coordinate )
  , m_direction( other.m_direction )
{
}

Thruster::~Thruster()
{
}

void
Thruster::register_item_to( Object& owner )
{
  m_shape = &owner.components.component< ShapeBehavior >().shape;
  m_physical_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
  owner.dispatcher.register_listener< ShipControl  >(
      std::bind( &Thruster::handle_command, this, std::placeholders::_1 ) );
  owner.dispatcher.register_listener< TimerUpdate  >(
      std::bind( &Thruster::handle_timer_update, this, std::placeholders::_1 ) );
  m_normalized_relative_coordinate = normalize( relative_coordinate() );
}


ObjectBehavior::Pointer
Thruster::clone() const
{
  return Pointer( new Thruster( *this ) );
}


void
Thruster::update( const ObjectBehavior& other )
{
  const Thruster& update( static_cast< const Thruster& >( other ) );
  m_jet = update.m_jet;
}


void
Thruster::handle_command( const yarrr::ShipControl& command )
{
  if ( command.type() != m_activation_command )
  {
    return;
  }

  m_jet.activate( command.timestamp() );
  apply_forces();
  force_synchronization();
}


void
Thruster::apply_forces()
{
  const auto relative_force_vector( vector_with<int64_t>( 180_degrees + m_direction, 1_metres ) );

  const int mass_and_engine_power_ratio( ( m_shape->mass() ? m_shape->mass() : 1 ) * 4096 );

  const int spinning_force_magic_constant{ 30 };

  m_physical_parameters->angular_velocity += z_of_cross_product(
      relative_coordinate(),
      relative_force_vector ) * spinning_force_magic_constant / ( mass_and_engine_power_ratio );

  auto pushing_force(
      relative_force_vector *
      strength_of_pushing_force( m_normalized_relative_coordinate, relative_force_vector, mass_and_engine_power_ratio ) );

  rotate( pushing_force, m_physical_parameters->orientation );

  m_physical_parameters->velocity += pushing_force;
}


void
Thruster::handle_timer_update( const yarrr::TimerUpdate& update ) const
{
  if ( !m_jet.is_active_at( update.timestamp ) )
  {
    return;
  }

  const auto thruster_coordinate( transform_center_of_mass_relative_to_absolute(
        relative_coordinate(),
        m_physical_parameters->coordinate,
        m_physical_parameters->orientation ) );

  const auto direction( m_direction + m_physical_parameters->orientation );

  m_particle_source.create(
      m_physical_parameters->timestamp,
      thruster_coordinate,
      //todo: calculate velocity of a given point ( add velocity from rotation... )
      m_physical_parameters->velocity + vector_with<int64_t>( direction, 25_metres ) );
}

void
Thruster::serialize_item( Serializer& serializer ) const
{
  m_jet.serialize( serializer );
  serializer.push_back( m_activation_command );
  serializer.push_back( m_direction );
}

void
Thruster::deserialize_item( Deserializer& deserializer )
{
  m_jet.deserialize( deserializer );
  m_activation_command = deserializer.pop_front< ShipControl::Type >();
  m_direction = deserializer.pop_front< Angle >();
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

