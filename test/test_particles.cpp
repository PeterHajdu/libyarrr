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

  const yarrr::Coordinate non_zero_velocity{ 10, -10 };
  yarrr::PhysicalParameters original_physical_parameters;
  const the::time::Time now{ original_physical_parameters.timestamp };
  const the::time::Time the_future{ original_physical_parameters.timestamp + 1000 };

  yarrr::PhysicalParameters physical_parameters_in_the_future;
  yarrr::Particle::Pointer particle;
  test::GraphicalEngine graphical_engine;
};

