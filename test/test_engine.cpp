#include <yarrr/engine.hpp>
#include "test_particle_factory.hpp"
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/particle.hpp>
#include <yarrr/bitmagic.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( an_engine )
{

  yarrr::Object::Pointer create_object()
  {
    std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior( new yarrr::PhysicalBehavior() );
    physical_parameters = &physical_behavior->physical_parameters;
    physical_parameters->coordinate = { 100, 200 };

    yarrr::Object::Pointer object( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );

    engine = new yarrr::Engine();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( engine ) );
    return object;
  }

  void SetUp()
  {
    object.reset( create_object().release() );

    particle_factory = static_cast< test::ParticleFactory* >( &the::ctci::service< yarrr::ParticleFactory >() );
    particle_factory->reset();
  }


  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::Engine::ctci ), Equals( true ) );
  }

  It( clones_the_id )
  {
    yarrr::ObjectBehavior::Pointer cloned_engine( engine->clone() );
    AssertThat( cloned_engine->id(), Equals( engine->id() ) );
  }

  It( updates_local_physical_state_when_a_command_arrives )
  {
    const yarrr::PhysicalParameters old_physical_parameters( *physical_parameters );
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );
    AssertThat( old_physical_parameters, !Equals( *physical_parameters ) );
  }

  void activate_and_wait_for_particles_on( yarrr::Object& object )
  {
    object.dispatcher.dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );
    object.dispatcher.dispatch( yarrr::TimerUpdate( 0 ) );
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
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::thruster, 0 ) );
    yarrr::Data serialized_engine( engine->clone()->serialize() );

    yarrr::Engine deserialized_engine;
    deserialized_engine.deserialize( serialized_engine );

    yarrr::Object::Pointer another_object( create_object().release() );

    another_object->dispatcher.dispatch( yarrr::TimerUpdate( 0 ) );
    AssertThat( particle_factory->was_particle_created, Equals( false ) );

    engine->update( deserialized_engine );
    another_object->dispatcher.dispatch( yarrr::TimerUpdate( 0 ) );
    AssertThat( particle_factory->was_particle_created, Equals( true ) );
  }

  test::ParticleFactory* particle_factory;
  yarrr::PhysicalParameters* physical_parameters;
  yarrr::Engine* engine;
  yarrr::Object::Pointer object;
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

