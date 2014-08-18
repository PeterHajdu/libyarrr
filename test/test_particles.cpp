#include <yarrr/particle.hpp>
#include "test_graphical_engine.hpp"
#include "test_particle_factory.hpp"
#include <yarrr/physical_parameters.hpp>
#include <thectci/service_registry.hpp>
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

Describe(a_particle_source)
{
  void SetUp()
  {
    particle_factory = static_cast< test::ParticleFactory* >( &the::ctci::service< yarrr::ParticleFactory >() );
    particle_factory->reset();
    source.reset( new yarrr::ParticleSource( deviation ) );
    particle_parameters.clear();

    for ( size_t i( 0 ); i < number_of_particles; ++i )
    {
      source->create( timestamp, center, velocity );
      particle_parameters.push_back( particle_factory->last_particle_parameters );
    }

    AssertThat( particle_factory->was_particle_created, Equals( true ) );
  }


  It( creates_particles_around_a_given_velocity_coordinate_timestamp )
  {
    for ( const auto& parameter : particle_parameters )
    {
      AssertThat( parameter.timestamp, Equals( timestamp ) );
      AssertThat( parameter.coordinate, Equals( center ) );
      const yarrr::Coordinate velocity_difference( parameter.velocity - velocity );
      AssertThat( yarrr::length_of( velocity_difference ), IsLessThan( length_deviation ) );
    }
  }

  bool not_all_velocity_is_equal() const
  {
    for ( const auto& parameter : particle_parameters )
    {
      if ( parameter.velocity != velocity )
      {
        return true;
      }
    }

    return false;
  }

  It( creates_particles_with_some_random )
  {
    AssertThat( not_all_velocity_is_equal(), Equals( true ) );
  }

  const size_t number_of_particles{ 100 };
  std::vector< yarrr::PhysicalParameters > particle_parameters;
  const the::time::Time timestamp{ 5000 };
  const yarrr::Coordinate center{ 100, 100 };
  const yarrr::Coordinate velocity{ 1000, 1000 };
  const int64_t deviation{ 5 };
  const int64_t length_deviation{ deviation * 2 };
  test::ParticleFactory* particle_factory;
  std::unique_ptr< yarrr::ParticleSource > source;
};

