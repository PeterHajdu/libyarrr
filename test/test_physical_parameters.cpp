#include <igloo/igloo_alt.h>
#include <yarrr/physical_parameters.hpp>

using namespace igloo;

Describe(an_object)
{
  void SetUp()
  {
    an_object.coordinate = start_position;
    an_object.velocity = start_velocity;
    an_object.angle = start_angle;
    an_object.vangle = -1000;
    an_object.timestamp = now;
  }

  It(moves)
  {
    yarrr::travel_in_time_to( future, an_object );
    AssertThat( an_object.coordinate, !Equals( start_position ) );
  }

  It(can_travel_back_in_time)
  {
    yarrr::Object object_state_now( an_object );
    yarrr::travel_in_time_to( past, an_object );
    AssertThat( an_object, !Equals( object_state_now ) );
    yarrr::travel_in_time_to( now, an_object );
    AssertThat( an_object, Equals( object_state_now ) );
  }

  It(rotates)
  {
    yarrr::travel_in_time_to( future, an_object );
    AssertThat( an_object.angle, !Equals( start_angle ) );
  }

  It(moves_more_during_a_longer_period)
  {
    yarrr::travel_in_time_to( future, an_object );
    const yarrr::Coordinate second_position( an_object.coordinate );
    const yarrr::Coordinate first_delta( second_position - start_position );
    yarrr::travel_in_time_to( future * 3, an_object );
    const yarrr::Coordinate third_position( an_object.coordinate );
    const yarrr::Coordinate second_delta( third_position - second_position );
    AssertThat( second_delta, IsGreaterThan( first_delta ) );
  }

  It(updates_its_timestamp_to_the_new_value)
  {
    yarrr::travel_in_time_to( future, an_object );
    AssertThat( an_object.timestamp, Equals( future ) );
  }

  yarrr::Object an_object;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 106 };
  const the::time::Clock::Time future{ 500000u };
  const the::time::Clock::Time now{ 10000u };
  const the::time::Clock::Time past{ 5000u };
};

