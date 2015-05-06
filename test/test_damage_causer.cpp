#include <yarrr/object_destroyed.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/shape.hpp>
#include <yarrr/shape_behavior.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_damage_causer )
{
  void create_shapeless_object()
  {
    object.reset( new yarrr::Object() );
    auto physical_behavior( std::make_unique< yarrr::PhysicalBehavior >() );
    integrity = &physical_behavior->physical_parameters.integrity;
    object->add_behavior( std::move( physical_behavior ) );
  }

  void add_shape_to_object()
  {
    yarrr::Shape shape;
    shape.add_tile( yarrr::Tile{ { 0, 0 }, { 0, 1 } } );
    object_mass = shape.mass();
    object->add_behavior( std::make_unique< yarrr::ShapeBehavior >( shape ) );
  }

  void add_damage_causer_to_object()
  {
    object->add_behavior( std::make_unique< yarrr::DamageCauser >() );
  }

  void SetUp()
  {
    create_shapeless_object();
    add_shape_to_object();
    add_damage_causer_to_object();

    was_object_destroyed_dispatched = false;
    object->dispatcher.register_listener< yarrr::ObjectDestroyed >(
        [ this ]( const yarrr::ObjectDestroyed& ){ was_object_destroyed_dispatched = true; } );
  }

  void collide()
  {
    object->dispatcher.dispatch( yarrr::Collide( dummy_object ) );
  }

  It( sets_the_integrity_of_a_shapeless_object_to_a_default_value )
  {
    create_shapeless_object();
    add_damage_causer_to_object();
    AssertThat( *integrity, Equals( default_integrity ) );
  }

  It( sets_integrity_to_the_mass_of_the_object_by_default )
  {
    AssertThat( *integrity, Equals( object_mass ) );
  }

  It( decreases_integrity_with_one_when_colliding )
  {
    collide();
    AssertThat( *integrity, Equals( object_mass - 1 ) );
  }

  void destroy_object()
  {
    for ( int i( 0 ); i < object_mass / 1; ++i )
    {
      AssertThat( was_object_destroyed_dispatched, Equals( false ) );
      collide();
    }
  }

  It( dispatches_object_destroyed_if_integrity_decreases_below_zero )
  {
    destroy_object();
    AssertThat( was_object_destroyed_dispatched, Equals( true ) );
  }

  It( resets_integrity_if_object_is_destroyed )
  {
    destroy_object();
    AssertThat( *integrity, Equals( object_mass ) );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  const int default_integrity{ 1 };

  yarrr::Object::Pointer object;
  yarrr::Object dummy_object;
  int16_t* integrity;

  bool was_object_destroyed_dispatched{ false };

  int object_mass;
};

