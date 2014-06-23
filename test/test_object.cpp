#include <igloo/igloo_alt.h>
#include <yarrr/object.hpp>

using namespace igloo;

Describe(an_object)
{
  void SetUp()
  {
    an_object.coordinate = start_position;
    an_object.velocity = start_velocity;
    an_object.angle = start_angle;
    an_object.vangle = -1;
    an_object.timestamp = 0u;
  }

  It(moves)
  {
    yarrr::advance_time_to( new_timestamp, an_object );
    AssertThat( an_object.coordinate, !Equals( start_position ) );
  }

  It(rotates)
  {
    yarrr::advance_time_to( new_timestamp, an_object );
    AssertThat( an_object.angle, !Equals( start_angle ) );
  }

  It(is_serializable)
  {
    const std::string binary_data( yarrr::serialize( an_object ) );
    const yarrr::Object deserialized_object( yarrr::deserialize( binary_data ) );
    AssertThat( deserialized_object, Equals( an_object ) );
  }

  It(moves_more_during_a_longer_period)
  {
    yarrr::advance_time_to( new_timestamp, an_object );
    const yarrr::Coordinate second_position( an_object.coordinate );
    const yarrr::Coordinate first_delta( second_position - start_position );
    yarrr::advance_time_to( new_timestamp * 3, an_object );
    const yarrr::Coordinate third_position( an_object.coordinate );
    const yarrr::Coordinate second_delta( third_position - second_position );
    AssertThat( second_delta, IsGreaterThan( first_delta ) );
  }

  It(updates_its_timestamp_to_the_new_value)
  {
    yarrr::advance_time_to( new_timestamp, an_object );
    AssertThat( an_object.timestamp, Equals( new_timestamp ) );
  }

  yarrr::Object an_object;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 105 };
  const the::time::Clock::Time new_timestamp{ 500000u };
};

