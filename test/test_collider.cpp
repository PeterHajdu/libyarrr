#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/object_identity.hpp>

#include "test_collider_bundle.hpp"

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_collider )
{

  void SetUp()
  {
    collider_bundle.reset( new test::ColliderBundle( yarrr::Collider::ship_layer ) );
    collider_bundle->add_shape();
    collider_bundle->shape->add_tile( yarrr::Tile{ { 0, 0 }, { 0, 0 } } );

    collider_on_same_layer.reset( new test::ColliderBundle( *collider_bundle, yarrr::Collider::ship_layer ) );
    colliding_bundle.reset( new test::ColliderBundle( *collider_bundle, yarrr::Collider::laser_layer ) );
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

  It( does_not_collide_on_the_same_layer )
  {
    collide_with( *collider_on_same_layer );
    AssertThat( collider_bundle->did_collide(), Equals( false ) )
    AssertThat( collider_on_same_layer->did_collide(), Equals( false ) )
  }

  void add_identity_to( const std::string& captain, test::ColliderBundle& collider )
  {
    collider.object.add_behavior( std::make_unique< yarrr::ObjectIdentity >( captain ) );
  }

  It( does_not_collide_with_the_same_captain )
  {
    const std::string same_captain{ "Black Beard" };
    add_identity_to( same_captain, *collider_bundle );
    add_identity_to( same_captain, *colliding_bundle );
    collide_with( *colliding_bundle );
    AssertThat( collider_bundle->did_collide(), Equals( false ) )
    AssertThat( colliding_bundle->did_collide(), Equals( false ) )
  }

  It( collides_with_another_captain )
  {
    add_identity_to( "a", *collider_bundle );
    add_identity_to( "b", *colliding_bundle );
    collide_with( *colliding_bundle );
    AssertThat( collider_bundle->did_collide(), Equals( true ) )
    AssertThat( colliding_bundle->did_collide(), Equals( true ) )
  }

  std::unique_ptr< test::ColliderBundle > collider_bundle;
  std::unique_ptr< test::ColliderBundle > colliding_bundle;
  std::unique_ptr< test::ColliderBundle > collider_on_same_layer;
};

