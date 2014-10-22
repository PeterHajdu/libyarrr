#include "test_services.hpp"
#include "test_synchronizable_behavior.hpp"
#include <yarrr/canon.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/shape_behavior.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( a_canon )
{

  void add_canon()
  {
    canons.push_back( new yarrr::Canon( { 0, 0 }, canon_orientation ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( canons.back() ) );
  }

  void SetUp()
  {
    bullets.clear();

    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::ShapeBehavior() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );

    inventory = &yarrr::component_of< yarrr::Inventory >( *object );
    yarrr::component_of< yarrr::PhysicalBehavior >( *object ).physical_parameters.orientation = ship_orientation;

    add_canon();

    was_canon_fired = false;

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::ObjectCreated >(
          [ this ]( const yarrr::ObjectCreated& object_created )
          {
            bullets.push_back( object_created.object->components.component< yarrr::PhysicalBehavior >().physical_parameters );
            was_canon_fired = true;
          } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( is_called_canon )
  {
    yarrr::Canon canon;
    AssertThat( canon.name(), Equals( "canon" ) );
  }

  It( is_synchronizable )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::Canon >();
  }

  It( creates_objects_only_for_fire_command )
  {
    object->dispatcher.dispatch( yarrr::ShipControl( yarrr::ShipControl::main_thruster, 0 ) );
    AssertThat( was_canon_fired, Equals( false ) );
  }

  It( creates_new_objects_when_canon_is_fired )
  {
    object->dispatcher.dispatch( yarrr::ShipControl( yarrr::ShipControl::fire, 0 ) );
    AssertThat( was_canon_fired, Equals( true ) );
  }

  It( registeres_itself_to_the_inventory )
  {
    AssertThat( inventory->items(), HasLength( 1u ) );
    AssertThat( &inventory->items().back().get(), Equals( canons.back() ) );
  }

  It( allows_more_canon_registration_to_the_same_object )
  {
    add_canon();
    AssertThat( inventory->items(), HasLength( 2u ) );
  }

  It( shoots_in_the_direction_of_the_canon )
  {
    object->dispatcher.dispatch( yarrr::ShipControl( yarrr::ShipControl::fire, 0 ) );
    const yarrr::PhysicalParameters& bullet_parameters( bullets.back() );

    AssertThat( bullet_parameters.orientation, Equals( canon_orientation + ship_orientation ) );
  }

  It( serializes_and_deserializes_orientation )
  {
    yarrr::Canon& original_canon( *canons.back() );
    const yarrr::Data serialized_canon( original_canon.clone()->serialize() );

    yarrr::Canon deserialized_canon;
    deserialized_canon.deserialize( serialized_canon );
    AssertThat( deserialized_canon.orientation(), Equals( canon_orientation ) );
  }

  bool was_canon_fired;

  yarrr::Inventory* inventory;

  const yarrr::Angle canon_orientation{ 10_degrees };
  const yarrr::Angle ship_orientation{ 100_degrees };

  yarrr::Object::Pointer object;
  std::vector< yarrr::Canon* > canons;
  std::vector< yarrr::PhysicalParameters > bullets;
  const yarrr::Coordinate zero{ 0, 0 };
};

