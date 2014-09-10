#include <igloo/igloo_alt.h>
#include <yarrr/physical_parameters.hpp>

using namespace igloo;

namespace
{
  bool are_almost_the_same( const yarrr::PhysicalParameters& l, const yarrr::PhysicalParameters& r )
  {
    const ssize_t allowed_angle_delta( 1 );
    return
      l.coordinate == r.coordinate &&
      l.velocity == r.velocity &&
      l.angular_velocity == r.angular_velocity &&
      l.timestamp == r.timestamp &&
      l.integrity == r.integrity &&
      std::abs( l.orientation - r.orientation ) <= allowed_angle_delta;
  }
}

Describe(physical_parameters)
{
  void SetUp()
  {
    physical_parameters.coordinate = start_position;
    physical_parameters.velocity = start_velocity;
    physical_parameters.orientation = start_angle;
    physical_parameters.angular_velocity = -1000;
    physical_parameters.timestamp = now;
  }

  It(moves)
  {
    yarrr::travel_in_time_to( future, physical_parameters );
    AssertThat( physical_parameters.coordinate, !Equals( start_position ) );
  }

  It(can_travel_back_in_time)
  {
    yarrr::PhysicalParameters object_state_now( physical_parameters );
    yarrr::travel_in_time_to( past, physical_parameters );
    AssertThat( physical_parameters, !Equals( object_state_now ) );
    yarrr::travel_in_time_to( now, physical_parameters );
    AssertThat( are_almost_the_same( physical_parameters, object_state_now ), Equals( true ) );
  }

  It(rotates)
  {
    yarrr::travel_in_time_to( future, physical_parameters );
    AssertThat( physical_parameters.orientation, !Equals( start_angle ) );
  }

  It(moves_more_during_a_longer_period)
  {
    yarrr::travel_in_time_to( future, physical_parameters );
    const yarrr::Coordinate second_position( physical_parameters.coordinate );
    const yarrr::Coordinate first_delta( second_position - start_position );
    yarrr::travel_in_time_to( future * 3, physical_parameters );
    const yarrr::Coordinate third_position( physical_parameters.coordinate );
    const yarrr::Coordinate second_delta( third_position - second_position );
    AssertThat( second_delta, IsGreaterThan( first_delta ) );
  }

  It(updates_its_timestamp_to_the_new_value)
  {
    yarrr::travel_in_time_to( future, physical_parameters );
    AssertThat( physical_parameters.timestamp, Equals( future ) );
  }

  yarrr::PhysicalParameters physical_parameters;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 106 };
  const the::time::Clock::Time future{ 500000u };
  const the::time::Clock::Time now{ 10000u };
  const the::time::Clock::Time past{ 5000u };
};

