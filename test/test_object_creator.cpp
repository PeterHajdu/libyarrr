#include <yarrr/object_creator.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/engine.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( ship_creator )
{

  void SetUp()
  {
    object = yarrr::create_ship();
  }

  It ( creates_objects_with_physical_parameters )
  {
    AssertThat( object->components.has_component< yarrr::PhysicalBehavior >(), Equals( true ) );
  }

  It ( creates_objects_with_an_engine )
  {
    AssertThat( object->components.has_component< yarrr::Engine >(), Equals( true ) );
  }

  It ( creates_objects_with_ship_graphics )
  {
    AssertThat( object->components.has_component< yarrr::ShipGraphics >(), Equals( true ) );
  }

  It ( creates_objects_with_a_canon )
  {
    AssertThat( object->components.has_component< yarrr::Canon >(), Equals( true ) );
  }

  It ( creates_objects_with_a_collider )
  {
    AssertThat( object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It ( creates_objects_with_a_damage_causer )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  It ( creates_objects_with_respawn_when_destroyed )
  {
    AssertThat( object->components.has_component< yarrr::RespawnWhenDestroyed >(), Equals( true ) );
  }

  It ( creates_objects_with_inventory )
  {
    AssertThat( object->components.has_component< yarrr::Inventory >(), Equals( true ) );
  }

  It ( creates_objects_with_loot_dropper )
  {
    AssertThat( object->components.has_component< yarrr::LootDropper >(), Equals( true ) );
  }

  yarrr::Object::Pointer object;
};

Describe( laser_creator )
{

  void SetUp()
  {
    ships_physical_parameters.vangle = 100;
    object = yarrr::create_laser( ships_physical_parameters );

    AssertThat( object->components.has_component< yarrr::PhysicalBehavior >(), Equals( true ) );
    laser_parameters = object->components.component<yarrr::PhysicalBehavior>().physical_parameters;
  }

  It ( creates_objects_with_laser_graphics )
  {
    AssertThat( object->components.has_component< yarrr::LaserGraphics >(), Equals( true ) );
  }

  It ( creates_objects_with_selfdestructor )
  {
    AssertThat( object->components.has_component< yarrr::SelfDestructor >(), Equals( true ) );
  }

  It ( creates_objects_that_do_not_spin )
  {
    AssertThat( laser_parameters.vangle, Equals( 0 ) );
  }

  It ( creates_objects_that_move_with_a_different_velocity )
  {
    AssertThat( laser_parameters.velocity, !Equals( ships_physical_parameters.velocity ) );
  }

  It ( creates_objects_with_a_collider )
  {
    AssertThat( object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It ( creates_objects_with_a_damage_causer )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  It ( creates_objects_with_delete_when_destroyed )
  {
    AssertThat( object->components.has_component< yarrr::DeleteWhenDestroyed >(), Equals( true ) );
  }

  yarrr::PhysicalParameters ships_physical_parameters;
  yarrr::PhysicalParameters laser_parameters;
  yarrr::Object::Pointer object;
};
