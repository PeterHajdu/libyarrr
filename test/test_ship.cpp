#include <igloo/igloo_alt.h>
#include <yarrr/ship.hpp>

using namespace igloo;

Describe(a_ship)
{
  void SetUp()
  {
    a_ship.coordinate = start_position;
    a_ship.velocity = start_velocity;
    a_ship.angle = start_angle;
    a_ship.vangle = -1;
  }

  It(moves)
  {
    yarrr::time_step( a_ship );
    AssertThat( a_ship.coordinate, !Equals( start_position ) );
  }

  It(rotates)
  {
    yarrr::time_step( a_ship );
    AssertThat( a_ship.angle, !Equals( start_angle ) );
  }

  It(is_serializable)
  {
    const std::string binary_data( yarrr::serialize( a_ship ) );
    const yarrr::Ship deserialized_ship( yarrr::deserialize( binary_data ) );
    AssertThat( deserialized_ship, Equals( a_ship ) );
  }

  yarrr::Ship a_ship;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 105 };
};

