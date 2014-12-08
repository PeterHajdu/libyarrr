#include <yarrr/thruster.hpp>
#include "test_particle_factory.hpp"
#include "test_synchronizable_behavior.hpp"
#include "test_remote_object.hpp"
#include <yarrr/object.hpp>
#include <yarrr/item.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/particle.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/timer_update.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( a_thruster )
{
  yarrr::Object::Pointer create_object_without_thruster()
  {
    std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior( new yarrr::PhysicalBehavior() );
    physical_parameters = &physical_behavior->physical_parameters;
    physical_parameters->coordinate = { 100, 200 };
    old_physical_parameters = *physical_parameters;
    yarrr::Object::Pointer object( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );
    object->add_behavior( yarrr::ShapeBehavior::Pointer( new yarrr::ShapeBehavior() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
    shape = &yarrr::component_of< yarrr::ShapeBehavior>( *object ).shape;
    shape->add_tile( yarrr::Tile( { 0, 0 }, { 0, 0 } ) );
    return object;
  }

  yarrr::Object::Pointer create_object_with_thruster()
  {
    yarrr::Object::Pointer object( create_object_without_thruster() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Thruster(
            activation_command,
            place_to_center_of_mass,
            face_right ) ) );
    thruster = &yarrr::component_of< yarrr::Thruster >( *object );
    return object;
  }

  void increase_mass_of_object( yarrr::Object& )
  {
    shape->add_tile( yarrr::Tile( { 0, 0 }, { 10, -10 } ) );
  }

  void SetUp()
  {
    object.reset( create_object_with_thruster().release() );

    particle_factory = static_cast< test::ParticleFactory* >( &the::ctci::service< yarrr::ParticleFactory >() );
    particle_factory->reset();
  }

  void activate_thruster_on( yarrr::Object& object )
  {
    object.dispatcher.dispatch( yarrr::ShipControl( activation_command, 0 ) );
  }

  It( is_an_item )
  {
    yarrr::Item& thruster_as_item( static_cast< yarrr::Item& >( *thruster ) );
    (void) thruster_as_item;
  }

  It( is_synchronizable )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::Thruster >();
  }

  It( updates_local_physical_state_when_the_activation_command_arrives )
  {
    activate_thruster_on( *object );
    AssertThat( old_physical_parameters, !Equals( *physical_parameters ) );
  }

  It( does_not_update_local_physical_state_if_it_is_not_the_activation_command )
  {
    object->dispatcher.dispatch( yarrr::ShipControl( non_activation_command, 0 ) );
    AssertThat( old_physical_parameters, Equals( *physical_parameters ) );
  }

  It( increases_the_velocity_but_does_not_spin_if_it_faces_center_of_mass )
  {
    auto object( create_object_without_thruster() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Thruster(
            activation_command,
            { -10, 0 },
            180_degrees ) ) );
    activate_thruster_on( *object );

    AssertThat( old_physical_parameters.velocity, !Equals( physical_parameters->velocity ) );
    AssertThat( old_physical_parameters.angular_velocity, Equals( physical_parameters->angular_velocity ) );
  }

  It( increases_the_velocity_in_proportion_to_object_mass )
  {
    auto object( create_object_without_thruster() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Thruster(
            activation_command,
            { -10_metres, 0 },
            180_degrees ) ) );

    activate_thruster_on( *object );
    const auto velocity_difference( physical_parameters->velocity - old_physical_parameters.velocity );
    old_physical_parameters = *physical_parameters;

    increase_mass_of_object( *object );

    activate_thruster_on( *object );
    const auto second_velocity_difference( physical_parameters->velocity - old_physical_parameters.velocity );

    AssertThat( second_velocity_difference, IsLessThan( velocity_difference ) );
  }


  It( increases_the_angular_velocity_but_does_not_push_if_it_is_perpendicular_to_center_of_mass )
  {
    auto object( create_object_without_thruster() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Thruster(
            activation_command,
            { -10_metres, 0 },
            90_degrees ) ) );
    activate_thruster_on( *object );

    AssertThat( old_physical_parameters.velocity, Equals( physical_parameters->velocity ) );
    AssertThat( old_physical_parameters.angular_velocity, !Equals( physical_parameters->angular_velocity ) );
  }


  It( increases_the_angular_velocity_in_proportion_to_object_mass )
  {
    auto object( create_object_without_thruster() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Thruster(
            activation_command,
            { -10_metres, 0 },
            90_degrees ) ) );
    activate_thruster_on( *object );
    const auto angular_velocity_difference(
        physical_parameters->angular_velocity -
        old_physical_parameters.angular_velocity );
    old_physical_parameters = *physical_parameters;

    increase_mass_of_object( *object );
    activate_thruster_on( *object );
    const auto second_angular_velocity_difference(
        physical_parameters->angular_velocity -
        old_physical_parameters.angular_velocity );

    AssertThat( second_angular_velocity_difference, IsLessThan( angular_velocity_difference ) );
  }

  void activate_and_wait_for_particles_on( yarrr::Object& object )
  {
    activate_thruster_on( object );
    object.dispatcher.dispatch( yarrr::TimerUpdate( 0 ) );
  }

  It( serializes_and_deserializes_activation_command_as_well )
  {
    auto another_object( test::create_remote_object_from( *object ) );
    activate_and_wait_for_particles_on( *another_object );
    AssertThat( particle_factory->was_particle_created, Equals( true ) );
  }

  It( creates_particles_when_it_is_on )
  {
    activate_and_wait_for_particles_on( *object );
    AssertThat( particle_factory->was_particle_created, Equals( true ) );
  }

  It( creates_particles_close_to_the_object )
  {
    activate_and_wait_for_particles_on( *object );
    yarrr::Coordinate coordinate_difference(
        particle_factory->last_particle_parameters.coordinate -
        physical_parameters->coordinate );
    yarrr::huplons_to_metres_in_place( coordinate_difference );

    AssertThat( yarrr::length_of( coordinate_difference ), IsLessThan( 30 ) );
  }

  It( serializes_and_deserializes_jet_states )
  {
    activate_thruster_on( *object );
    yarrr::Data serialized_thruster( thruster->clone()->serialize() );
    yarrr::Thruster deserialized_thruster;
    deserialized_thruster.deserialize( serialized_thruster );
    yarrr::Object::Pointer another_object( create_object_with_thruster().release() );
    another_object->dispatcher.dispatch( yarrr::TimerUpdate( 0 ) );
    AssertThat( particle_factory->was_particle_created, Equals( false ) );
    thruster->update( deserialized_thruster );
    another_object->dispatcher.dispatch( yarrr::TimerUpdate( 0 ) );
    AssertThat( particle_factory->was_particle_created, Equals( true ) );
  }

  It( synchronizes_only_rarely_and_if_something_happens )
  {
    AssertThat( thruster->should_synchronize(), Equals( true ) );
    AssertThat( thruster->should_synchronize(), Equals( false ) );
    activate_thruster_on( *object );
    AssertThat( thruster->should_synchronize(), Equals( true ) );
  }

  test::ParticleFactory* particle_factory;
  yarrr::PhysicalParameters* physical_parameters;
  yarrr::PhysicalParameters old_physical_parameters;
  yarrr::Shape* shape;
  yarrr::Thruster* thruster;
  yarrr::Object::Pointer object;
  const yarrr::ShipControl::Type activation_command{ yarrr::ShipControl::starboard_thruster };
  const yarrr::ShipControl::Type non_activation_command{ yarrr::ShipControl::port_thruster };

  const yarrr::Tile::Coordinate place_to_center_of_mass{ 0, 0 };
  const yarrr::Angle face_right{ 0 };
};


Describe( a_jet )
{
  void SetUp()
  {
    jet.activate( now );
  }

  It( is_active_until_a_cooldown_time )
  {
    AssertThat( jet.is_active_at( now ), Equals( true ) );
    AssertThat( jet.is_active_at( now + yarrr::Jet::cooldown_time ), Equals( false ) );
  }

  bool is_a_copy( const yarrr::Jet& jet ) const
  {
    return
      jet.is_active_at( now ) == true &&
      jet.is_active_at( now + yarrr::Jet::cooldown_time ) == false;
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::Data serialized_jet;
    yarrr::Serializer serializer( serialized_jet );
    jet.serialize( serializer );

    yarrr::Deserializer deserializer( serialized_jet );
    yarrr::Jet deserialized_jet;
    deserialized_jet.deserialize( deserializer );

    AssertThat( is_a_copy( deserialized_jet ), Equals( true ) );
  }

  It( is_assignable )
  {
    yarrr::Jet another_jet;
    another_jet = jet;
    AssertThat( is_a_copy( another_jet ), Equals( true ) );
  }

  It( is_copyconstructable )
  {
    yarrr::Jet another_jet( jet );
    AssertThat( is_a_copy( another_jet ), Equals( true ) );
  }

  yarrr::Jet jet;
  const the::time::Time now{ 1000 };
};

