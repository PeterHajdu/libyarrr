#include "test_services.hpp"
#include "test_synchronizable_behavior.hpp"
#include <yarrr/canon.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/command.hpp>
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
    canons.push_back( new yarrr::Canon() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( canons.back() ) );
  }

  void SetUp()
  {
    bullets.clear();

    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::ShapeBehavior() ) );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
    inventory = &object->components.component< yarrr::Inventory >();

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
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::main_thruster, 0 ) );
    AssertThat( was_canon_fired, Equals( false ) );
  }

  It( creates_new_objects_when_canon_is_fired )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::fire, 0 ) );
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

  It( places_canons_apart )
  {
    add_canon();
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::fire, 0 ) );
    AssertThat( bullets, HasLength( 2u ) );
    const yarrr::Coordinate canon_coordinate_difference(
        bullets[ 0 ].coordinate - bullets[ 1 ].coordinate );

    AssertThat( canon_coordinate_difference, !Equals( zero ) );
  }

  bool was_canon_fired;

  yarrr::Inventory* inventory;

  yarrr::Object::Pointer object;
  std::vector< yarrr::Canon* > canons;
  std::vector< yarrr::PhysicalParameters > bullets;
  const yarrr::Coordinate zero{ 0, 0 };
};

