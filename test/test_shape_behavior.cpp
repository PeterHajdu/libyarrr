#include "test_synchronizable_behavior.hpp"
#include <yarrr/shape_behavior.hpp>
#include <yarrr/shape.hpp>
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/collider.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_shape_behavior )
{

  yarrr::Shape create_shape()
  {
    yarrr::Shape a_shape;
    a_shape.add_tile( a_tile );
    a_shape.add_tile( another_tile );
    return a_shape;
  }

  void SetUp()
  {
    shape_behavior.reset( new yarrr::ShapeBehavior( create_shape() ) );
  }

  It( is_synchronizable )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::ShapeBehavior >();
  }

  It( clones_the_same_shape )
  {
    yarrr::ObjectBehavior::Pointer clone( shape_behavior->clone() );
    AssertThat( static_cast< const yarrr::ShapeBehavior& >( *clone ).shape, Equals( shape_behavior->shape ) );
  }

  It( serializes_and_deserializes_the_same_shape )
  {
    const yarrr::Data serialized_data( shape_behavior->serialize() );
    yarrr::ShapeBehavior deserialized_shape_behavior;
    deserialized_shape_behavior.deserialize( serialized_data );
    AssertThat( deserialized_shape_behavior.shape, Equals( shape_behavior->shape ) );
  }

  It( initiates_integrity_recalculation_if_damage_causer_exists )
  {
    yarrr::Object an_object;
    auto physical_behavior( std::make_unique< yarrr::PhysicalBehavior >() );
    auto& integrity( physical_behavior->physical_parameters.integrity );
    an_object.add_behavior( std::move( physical_behavior ) );
    an_object.add_behavior( std::make_unique< yarrr::DamageCauser >() );

    auto shape( create_shape() );
    auto mass( shape.mass() );
    AssertThat( integrity, !Equals( mass ) );

    an_object.add_behavior( std::make_unique< yarrr::ShapeBehavior >( shape ) );
    AssertThat( integrity, Equals( mass ) );
  }

  const yarrr::Tile a_tile{ { 0, 0 }, { 1, 1 } };
  const yarrr::Tile another_tile{ { 2, 2 }, { 3, 3 } };
  std::unique_ptr< yarrr::ShapeBehavior > shape_behavior;
};

