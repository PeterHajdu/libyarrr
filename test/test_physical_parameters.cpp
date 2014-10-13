#include <igloo/igloo_alt.h>
#include <yarrr/physical_parameters.hpp>

using namespace igloo;

namespace
{
  bool is_normal_angle( const yarrr::Angle& angle )
  {
    const bool is_normal_angle( angle > -360_degrees && angle < 360_degrees );
    if ( !is_normal_angle )
    {
      std::cout << "angle: " << angle << std::endl;
    }

    return is_normal_angle;
  }

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
    physical_parameters.angular_velocity = start_angular_velocity;
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

  It(normalizes_orientation)
  {
    physical_parameters.orientation = 359_degrees;
    physical_parameters.angular_velocity = 100_degrees;
    yarrr::travel_in_time_to( future, physical_parameters );
    AssertThat( is_normal_angle( physical_parameters.orientation ), Equals( true ) );

    physical_parameters.timestamp = now;
    physical_parameters.orientation = -359_degrees;
    physical_parameters.angular_velocity = -100_degrees;
    yarrr::travel_in_time_to( future, physical_parameters );
    AssertThat( is_normal_angle( physical_parameters.orientation ), Equals( true ) );
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

  It(can_be_averaged_with_weight)
  {
    yarrr::PhysicalParameters another_physical_parameters;
    another_physical_parameters.coordinate = { 1000, 2000 };
    another_physical_parameters.velocity = { 1000, 2000 };
    another_physical_parameters.orientation = 10;
    another_physical_parameters.angular_velocity = 10;

    yarrr::PhysicalParameters average( yarrr::weight_arithmetic_mean(
        physical_parameters,
        another_physical_parameters,
        0.9 ) );

    AssertThat(
        yarrr::length_of( average.coordinate - start_position ), IsLessThan(
        yarrr::length_of( average.coordinate - another_physical_parameters.coordinate ) ) );

    AssertThat(
        std::abs( average.orientation - start_angle ), IsLessThan(
        std::abs( average.orientation - another_physical_parameters.orientation ) ) );

    //velocities are exceptions
    AssertThat(
        yarrr::length_of( average.velocity - start_velocity ), Equals(
        yarrr::length_of( average.velocity - another_physical_parameters.velocity ) ) );


    AssertThat(
        std::abs( average.angular_velocity - start_angular_velocity ), Equals(
        std::abs( average.angular_velocity - another_physical_parameters.angular_velocity ) ) );

    AssertThat( average.timestamp, Equals( physical_parameters.timestamp ) );
    AssertThat( average.integrity, Equals( physical_parameters.integrity ) );
  }

  yarrr::PhysicalParameters physical_parameters;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 106 };
  const yarrr::Angle start_angular_velocity{ -1100 };
  const the::time::Clock::Time future{ 500000u };
  const the::time::Clock::Time now{ 10000u };
  const the::time::Clock::Time past{ 5000u };
};

