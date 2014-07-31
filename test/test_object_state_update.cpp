#include <yarrr/object_state_update.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/entity_factory.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(an_object_state_update)
{
  void SetUp()
  {
    physical_parameters.coordinate = start_position;
    physical_parameters.velocity = start_velocity;
    physical_parameters.angle = start_angle;
    physical_parameters.vangle = -1000;
    physical_parameters.timestamp = now;
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ObjectStateUpdate::ctci ), Equals( true ) );
  }

  It( can_recreate_the_object_from_the_deserialized_form )
  {
    yarrr::ObjectStateUpdate state_update( physical_parameters );
    yarrr::Data serialized_object_state_update( state_update.serialize() );

    yarrr::ObjectStateUpdate deserialized_object_state_update;
    deserialized_object_state_update.deserialize( serialized_object_state_update );

    AssertThat( deserialized_object_state_update.physical_parameters(), Equals( physical_parameters ) );
  }

  yarrr::PhysicalParameters physical_parameters;
  const yarrr::Coordinate start_position{ 101, 102 };
  const yarrr::Velocity start_velocity{ 103, 104 };
  const yarrr::Angle start_angle{ 106 };
  const the::time::Clock::Time future{ 500000u };
  const the::time::Clock::Time now{ 10000u };
  const the::time::Clock::Time past{ 5000u };
};

