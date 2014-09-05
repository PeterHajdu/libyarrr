#include "test_services.hpp"
#include <yarrr/canon.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/command.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( a_canon )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
    inventory = &object->components.component< yarrr::Inventory >();

    physical_behavior.register_to( *object );
    canon.reset( new yarrr::Canon() );
    canon->register_to( *object );

    was_canon_fired = false;

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::ObjectCreated >(
          [ this ]( const yarrr::ObjectCreated& )
          {
            was_canon_fired = true;
          } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( creates_objects_only_for_fire_command )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::cw, 0 ) );
    AssertThat( was_canon_fired, Equals( false ) );
  }

  It( creates_new_objects_when_canon_is_fired )
  {
    object->dispatcher.dispatch( yarrr::Command( yarrr::Command::fire, 0 ) );
    AssertThat( was_canon_fired, Equals( true ) );
  }

  It( registeres_itself_to_the_inventory )
  {
    AssertThat( inventory->items(), HasLength( 1 ) );
    AssertThat( &inventory->items().back().get(), Equals( canon.get() ) );
  }

  bool was_canon_fired;

  yarrr::Inventory* inventory;
  yarrr::PhysicalBehavior physical_behavior;
  std::unique_ptr< yarrr::Canon > canon;

  yarrr::Object::Pointer object;
};

