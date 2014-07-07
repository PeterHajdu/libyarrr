#include <yarrr/object_state_update.hpp>
#include <yarrr/object.hpp>
#include <yarrr/event_factory.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(an_object_state_update)
{
  void SetUp()
  {
    an_object.coordinate = start_position;
    an_object.velocity = start_velocity;
    an_object.angle = start_angle;
    an_object.vangle = -1000;
    an_object.timestamp = now;
  }

  It( is_registered_to_event_factory )
  {
    AssertThat( yarrr::EventFactory::is_registered( yarrr::ObjectStateUpdate::ctci ), Equals( true ) );
  }

  It( can_recreate_the_object_from_the_deserialized_form )
  {
    yarrr::ObjectStateUpdate state_update( an_object );
    yarrr::Data serialized_object_state_update( state_update.serialize() );

    yarrr::ObjectStateUpdate deserialized_object_state_update;
    deserialized_object_state_update.deserialize( serialized_object_state_update );

    AssertThat( deserialized_object_state_update.object(), Equals( an_object ) );
  }

  yarrr::Object an_object;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 106 };
  const the::time::Clock::Time future{ 500000u };
  const the::time::Clock::Time now{ 10000u };
  const the::time::Clock::Time past{ 5000u };
};

