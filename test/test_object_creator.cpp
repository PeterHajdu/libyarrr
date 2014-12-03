#include <yarrr/object_creator.hpp>
#include <yarrr/object_identity.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/thruster.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/shape_behavior.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( laser_creator )
{

  void SetUp()
  {
    ships_physical_parameters.angular_velocity = 100;

    const auto identity( std::make_unique< yarrr::ObjectIdentity >( originator ) );
    object = yarrr::create_laser( ships_physical_parameters, *identity );

    AssertThat( object->components.has_component< yarrr::PhysicalBehavior >(), Equals( true ) );
    laser_parameters = object->components.component<yarrr::PhysicalBehavior>().physical_parameters;
  }

  It ( creates_objects_with_the_originators_object_identity )
  {
    AssertThat( yarrr::has_component< yarrr::ObjectIdentity >( *object ), Equals( true ) );
    AssertThat( yarrr::component_of< yarrr::ObjectIdentity >( *object ).captain(), Equals( originator ) );
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
    AssertThat( laser_parameters.angular_velocity, Equals( 0 ) );
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

  yarrr::PhysicalParameters ships_physical_parameters;
  yarrr::PhysicalParameters laser_parameters;
  yarrr::Object::Pointer object;
  const std::string originator{ "Black Beard" };
};

Describe( loot_creator )
{
  void SetUp()
  {
    object = yarrr::create_loot( loots_physical_parameters, canon );
  }

  It( creates_objects_with_inventory )
  {
    AssertThat( object->components.has_component< yarrr::Inventory >(), Equals( true ) );
  }

  It( creates_objects_with_one_item_of_the_owner )
  {
    AssertThat( object->components.component< yarrr::Inventory >().items(), HasLength( 1u ) );
  }

  It( creates_objects_with_collider )
  {
    AssertThat( object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It( creates_destructable_objects )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  It( creates_objects_with_loot_attacher )
  {
    AssertThat( object->components.has_component< yarrr::LootAttacher >(), Equals( true ) );
  }

  It( creates_a_self_destructing_object )
  {
    AssertThat( object->components.has_component< yarrr::SelfDestructor >(), Equals( true ) );
  }

  It ( creates_objects_with_shape_graphics_with_at_least_one_tile )
  {
    AssertThat( object->components.has_component< yarrr::ShapeGraphics >(), Equals( true ) );
    AssertThat( yarrr::component_of< yarrr::ShapeBehavior >( *object ).shape.tiles().size(), !Equals( 0u ) );
  }

  yarrr::PhysicalParameters loots_physical_parameters;
  yarrr::Canon canon;
  yarrr::Object::Pointer object;
};

