#include <yarrr/object_decorator.hpp>
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/ship_control.hpp>

#include "test_behavior.hpp"

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_object_decorator )
{
  void SetUp()
  {
    last_bullet_direction = 0_degrees;
    object = std::make_unique< yarrr::Object >();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );

    object->dispatcher.register_listener< yarrr::Fire >(
        [ this ]( const yarrr::Fire& fire )
        {
          last_bullet_direction = fire.direction;
        });

    current_position = &yarrr::coordinate_of( *object );
    current_velocity = &yarrr::velocity_of( *object );
    decorator = std::make_unique< yarrr::ObjectDecorator >( *object );
  }

  It( has_the_id_of_the_object_as_string )
  {
    const std::string id_as_string{ decorator->id() };
    AssertThat( id_as_string, Equals( std::to_string( object->id() ) ) );
  }

  It( can_add_a_behavior_clone_to_the_object )
  {
    yarrr::ObjectBehavior::Pointer behavior( new test::Behavior() );
    decorator->add_behavior_clone( *behavior );
    AssertThat( yarrr::has_component< test::Behavior >( *object ), Equals( true ) );
    AssertThat( yarrr::component_of< test::Behavior >( *object ).id(), Equals( behavior->id() ) );
  }

  It( can_move_an_object_to_another_place )
  {
    const yarrr::Coordinate new_position{ *current_position + yarrr::Coordinate{ 100, 200 } };
    decorator->move_to( new_position );
    AssertThat( *current_position, Equals( new_position ) );
  }

  It( can_set_an_objects_velocity )
  {
    const yarrr::Velocity new_velocity{ *current_velocity + yarrr::Velocity{ 100, 200 } };
    decorator->set_velocity( new_velocity );
    AssertThat( *current_velocity, Equals( new_velocity ) );
  }

  It( can_shoot_in_a_given_direction )
  {
    const yarrr::Angle direction{ 10_degrees };
    decorator->fire( direction );
    AssertThat( last_bullet_direction, Equals( direction ) );
  }

  yarrr::Coordinate* current_position;
  yarrr::Coordinate* current_velocity;
  yarrr::Object::Pointer object;
  std::unique_ptr< yarrr::ObjectDecorator > decorator;
  yarrr::Angle last_bullet_direction;
};

