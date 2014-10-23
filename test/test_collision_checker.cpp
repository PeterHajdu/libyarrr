#include <yarrr/collision_checker.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>

#include "test_collider_bundle.hpp"

#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{

int unique_collision_layer()
{
  static int last_layer{ 0 };
  return last_layer++;
}

}

Describe( a_collision_checker )
{
  Describe( two_shapeless_object_collision )
  {

    It( should_not_collide_two_shapeless_objects )
    {
      object_a.collider->collide_if_needed_with( *object_b.collider );
      AssertThat( object_a.did_collide(), Equals( false ) );
      AssertThat( object_b.did_collide(), Equals( false ) );
    }

    test::ColliderBundle object_a{ unique_collision_layer() };
    test::ColliderBundle object_b{ object_a, unique_collision_layer() };
  };

  Describe( shape_shape_collision )
  {
    void SetUp()
    {
      object_a.reset( new test::ColliderBundle( unique_collision_layer() ) );
      object_a->add_shape();
      object_a->shape->add_tile( yarrr::Tile{ { -10, 0 }, { 10, 0 } } );

      object_b.reset( new test::ColliderBundle( *object_a, unique_collision_layer() ) );
      object_b->add_shape();
      object_b->shape->add_tile( yarrr::Tile{ { -5, 0 }, { 5, 0 } } );
    }

    bool are_colliders()
    {
      object_a->collider->collide_if_needed_with( *object_b->collider );
      return object_a->did_collide() && object_b->did_collide();
    }

    It( does_not_collide_if_they_are_further_than_the_sum_of_their_radius )
    {
      object_b->physical_parameters().coordinate.y += object_a->shape->radius() + object_b->shape->radius();
      AssertThat( are_colliders(), Equals( false ) );
    }

    It( collides_if_they_are_closer_than_the_sum_of_their_radius )
    {
      object_b->physical_parameters().coordinate.y += object_a->shape->radius() + object_b->shape->radius() - 1;
      AssertThat( are_colliders(), Equals( true ) );
    }

    test::ColliderBundle::Pointer object_a;
    test::ColliderBundle::Pointer object_b;
  };

  Describe( point_with_shape_collision )
  {
    void SetUp()
    {
      object_with_shape.reset( new test::ColliderBundle( unique_collision_layer() ) );
      object_with_shape->add_shape();
      object_with_shape->shape->add_tile( yarrr::Tile{ { -10, 0 }, { 10, 0 } } );

      object_without_shape.reset( new test::ColliderBundle( *object_with_shape, unique_collision_layer() ) );
    }

    bool are_colliders()
    {
      object_with_shape->collider->collide_if_needed_with( *object_without_shape->collider );
      return object_with_shape->did_collide() && object_without_shape->did_collide();
    }

    It( does_not_collide_if_the_point_is_closer_than_the_radius_but_outside_any_tiles )
    {
      object_without_shape->physical_parameters().coordinate.y += 3 * yarrr::Tile::unit_length;
      AssertThat( are_colliders(), Equals( false ) );
    }

    It( collides_if_the_point_is_in_any_of_the_tiles )
    {
      AssertThat( are_colliders(), Equals( true ) );
    }

    test::ColliderBundle::Pointer object_with_shape;
    test::ColliderBundle::Pointer object_without_shape;
  };

};

