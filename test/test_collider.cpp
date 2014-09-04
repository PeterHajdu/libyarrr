#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>

#include "test_collider_bundle.hpp"

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_collider )
{

  void SetUp()
  {
    collider_bundle.reset( new test::ColliderBundle( yarrr::Collider::ship_layer ) );
    collider_on_same_layer.reset( new test::ColliderBundle( *collider_bundle, yarrr::Collider::ship_layer ) );
    colliding_bundle.reset( new test::ColliderBundle( *collider_bundle, yarrr::Collider::laser_layer ) );
    collider_too_far.reset( new test::ColliderBundle( *collider_bundle, yarrr::Collider::laser_layer ) );
    collider_too_far->move_far_enough();
  }

  void collide_with( test::ColliderBundle& other )
  {
    collider_bundle->collider->collide_if_needed_with( *other.collider );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat(
        &collider_bundle->object.components.component< yarrr::Collider >(),
        Equals( collider_bundle->collider ) );
  }

  It( dispatches_a_collide_object_on_both_objects )
  {
    collide_with( *colliding_bundle );
    AssertThat( colliding_bundle->did_collide(), Equals( true ) )
  }

  It( dispatches_a_collide_object_containing_the_other_on_both_objects )
  {
    collide_with( *colliding_bundle );
    AssertThat( colliding_bundle->did_collide(), Equals( true ) )
    AssertThat( collider_bundle->did_collide(), Equals( true ) )
    AssertThat( colliding_bundle->collided_with, Equals( &collider_bundle->object ) );
    AssertThat( collider_bundle->collided_with, Equals( &colliding_bundle->object ) );
  }

  It( does_not_collide_if_objects_are_far_from_eachother )
  {
    collide_with( *collider_too_far );
    AssertThat( collider_bundle->did_collide(), Equals( false ) )
    AssertThat( colliding_bundle->did_collide(), Equals( false ) )
  }

  It( does_not_collide_on_the_same_layer )
  {
    collide_with( *collider_on_same_layer );
    AssertThat( collider_bundle->did_collide(), Equals( false ) )
    AssertThat( collider_on_same_layer->did_collide(), Equals( false ) )
  }

  std::unique_ptr< test::ColliderBundle > collider_bundle;
  std::unique_ptr< test::ColliderBundle > colliding_bundle;
  std::unique_ptr< test::ColliderBundle > collider_too_far;
  std::unique_ptr< test::ColliderBundle > collider_on_same_layer;
};

