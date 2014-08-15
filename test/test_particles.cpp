#include <yarrr/particle.hpp>
#include "test_graphical_engine.hpp"
#include <yarrr/physical_parameters.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_particle)
{
  void SetUp()
  {
    original_physical_parameters.velocity = non_zero_velocity;
    particle.reset( new yarrr::Particle(
          original_physical_parameters,
          graphical_engine ) );
    graphical_engine.draw_objects();

    physical_parameters_in_the_future = original_physical_parameters;
    yarrr::travel_in_time_to( the_future, physical_parameters_in_the_future );
  }

  It( is_a_graphical_object )
  {
    yarrr::GraphicalObject& graphical_particle( *particle );
    (void)graphical_particle;
  }

  It( draws_a_particle_with_its_physical_parameters_and_age )
  {
    AssertThat( graphical_engine.last_drawn_particle, Equals( original_physical_parameters ) );
    AssertThat( graphical_engine.age_of_last_drawn_particle, Equals( 0u ) );
  }

  It( can_travel_in_time_to_a_given_point )
  {
    particle->travel_in_time_to( the_future );
    graphical_engine.draw_objects();
    AssertThat( graphical_engine.last_drawn_particle, Equals( physical_parameters_in_the_future ) );
    AssertThat( graphical_engine.age_of_last_drawn_particle, Equals( the_future - now ) );
  }

  It( lives_only_for_a_few_seconds )
  {
    particle->travel_in_time_to( the_future );
    AssertThat( particle->is_alive(), Equals( false ) );
  }

  const yarrr::Coordinate non_zero_velocity{ 10, -10 };
  yarrr::PhysicalParameters original_physical_parameters;
  const the::time::Time now{ original_physical_parameters.timestamp };
  const the::time::Time three_seconds{ 3000000 };
  const the::time::Time the_future{ original_physical_parameters.timestamp + three_seconds };

  yarrr::PhysicalParameters physical_parameters_in_the_future;
  yarrr::Particle::Pointer particle;
  test::GraphicalEngine graphical_engine;
};


Describe( a_particle_container )
{

  void SetUp()
  {
    original_physical_parameters.velocity = non_zero_velocity;
    container.reset( new yarrr::ParticleContainer() );
    container->push_back( yarrr::Particle::Pointer( new yarrr::Particle(
          original_physical_parameters,
          graphical_engine ) ) );
  }

  It( moves_particles )
  {
    container->travel_in_time_to( not_so_far_future );
    graphical_engine.draw_objects();
    AssertThat( graphical_engine.last_drawn_particle, !Equals( original_physical_parameters ) );
  }

  It( can_tell_the_number_of_particles_contained )
  {
    AssertThat( yarrr::ParticleContainer(), IsEmpty() );
    AssertThat( *container, HasLength( 1u ) );
  }

  It( deletes_dead_particles_if_it_contains_only_one_particle )
  {
    container->travel_in_time_to( far_future );
    AssertThat( *container, IsEmpty() );
  }

  It( deletes_dead_particles_if_it_contains_more_particles )
  {
    yarrr::PhysicalParameters alive_parameters( original_physical_parameters );
    alive_parameters.timestamp+=three_seconds;
    container->push_back( yarrr::Particle::Pointer( new yarrr::Particle(
            alive_parameters,
            graphical_engine ) ) );

    container->travel_in_time_to( far_future );
    AssertThat( *container, HasLength( 1u ) );
  }

  It( works_on_empty_container )
  {
    yarrr::ParticleContainer empty_container;
    empty_container.travel_in_time_to( far_future );
  }

  std::unique_ptr< yarrr::ParticleContainer > container;

  const yarrr::Coordinate non_zero_velocity{ 10, -10 };
  yarrr::PhysicalParameters original_physical_parameters;
  const the::time::Time now{ original_physical_parameters.timestamp };

  const the::time::Time one_second{ 1000000 };
  const the::time::Time not_so_far_future{ original_physical_parameters.timestamp + one_second };

  const the::time::Time three_seconds{ 3000000 };
  const the::time::Time far_future{ original_physical_parameters.timestamp + three_seconds };

  yarrr::Particle::Pointer particle;

  test::GraphicalEngine graphical_engine;
};

