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
  }

  It(moves)
  {
    yarrr::time_step( an_object );
    AssertThat( an_object.coordinate, !Equals( start_position ) );
  }

  It(rotates)
  {
    yarrr::time_step( an_object );
    AssertThat( an_object.angle, !Equals( start_angle ) );
  }

  It(is_serializable)
  {
    const std::string binary_data( yarrr::serialize( an_object ) );
    const yarrr::Object deserialized_object( yarrr::deserialize( binary_data ) );
    AssertThat( deserialized_object, Equals( an_object ) );
  }

  yarrr::Object an_object;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 105 };
};

