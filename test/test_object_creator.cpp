#include <yarrr/object_creator.hpp>
#include <yarrr/object_identity.hpp>
#include <yarrr/loot.hpp>
#include <yarrr/cargo.hpp>
#include <yarrr/basic_behaviors.hpp>
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
    object = yarrr::create_loot( loots_physical_parameters, tile, goods );
  }

  It( creates_objects_with_collider )
  {
    AssertThat( object->components.has_component< yarrr::Collider >(), Equals( true ) );
  }

  It( creates_destructable_objects )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  It( creates_objects_with_cargo_space )
  {
    AssertThat( object->components.has_component< yarrr::CargoSpace >(), Equals( true ) );
  }

  It( creates_objects_with_the_loot_passed )
  {
    AssertThat( yarrr::component_of< yarrr::CargoSpace >( *object ).goods(), Contains( good ) );
  }

  It( creates_objects_with_loot_attacher )
  {
    AssertThat( object->components.has_component< yarrr::LootAttacher >(), Equals( true ) );
  }

  It( creates_a_self_destructing_object )
  {
    AssertThat( object->components.has_component< yarrr::SelfDestructor >(), Equals( true ) );
  }

  It ( creates_objects_with_shape_graphics_with_the_given_tile )
  {
    AssertThat( object->components.has_component< yarrr::ShapeGraphics >(), Equals( true ) );
    AssertThat( yarrr::component_of< yarrr::ShapeBehavior >( *object ).shape.tiles().size(), Equals( 1u ) );
    AssertThat( yarrr::component_of< yarrr::ShapeBehavior >( *object ).shape.tiles().back(), Equals( tile ) );
  }

  const yarrr::Goods good{ "a", "b", 10 };
  const std::vector< yarrr::Goods > goods{ good };
  const yarrr::Tile tile{ { 1, 1 }, { 2, 2 } };
  yarrr::PhysicalParameters loots_physical_parameters;
  yarrr::Object::Pointer object;
};

