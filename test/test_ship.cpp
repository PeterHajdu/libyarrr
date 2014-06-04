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

  yarrr::Ship a_ship;
  const yarrr::Coordinate start_position{ 100, 100 };
  const yarrr::Velocity start_velocity{ 100, 100 };
  const yarrr::Angle start_angle{ 100 };
};

