#include <yarrr/object_creator.hpp>
#include <yarrr/object.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/thruster.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/entity_factory.hpp>

#include <random>

namespace
{

const int laser_speed{ 400_metres };

std::random_device random_device;
std::default_random_engine random_engine( random_device() );
std::uniform_int_distribution<int> distribution{ -75_metres, 75_metres };

void add_random_velocity_to( yarrr::PhysicalParameters& physical_parameters )
{
  physical_parameters.velocity += yarrr::Coordinate( distribution( random_engine ), distribution( random_engine ) );
}

}


namespace yarrr
{

Object::Pointer
create_loot( PhysicalParameters new_physical_parameters, const ObjectBehavior& item )
{
  add_random_velocity_to( new_physical_parameters );
  Object::Pointer loot( new Object() );
  loot->add_behavior( ObjectBehavior::Pointer( new Inventory() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new PhysicalBehavior( new_physical_parameters ) ) );
  loot->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::loot_layer ) ) );
  loot->add_behavior( ObjectBehavior::Pointer( new DeleteWhenDestroyed() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 30 ) ) );
  loot->add_behavior( ObjectBehavior::Pointer( new LootAttacher() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new SelfDestructor( loot->id, 36000000u ) ) );

  ShapeBehavior* shape( new ShapeBehavior() );
  //todo: should this be the wreck?
  shape->shape.add_tile( Tile{ { 0, 0 }, { 0, 0 } } );
  loot->add_behavior( ObjectBehavior::Pointer( shape ) );
  loot->add_behavior( ObjectBehavior::Pointer( new ShapeGraphics() ) );

  loot->add_behavior( item.clone() );

  return loot;
}


Object::Pointer
create_ship()
{
  Object::Pointer ship( new Object() );
  ship->add_behavior( ObjectBehavior::Pointer( new Inventory() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new PhysicalBehavior() ) );

  ShapeBehavior* shape( new ShapeBehavior() );
  //todo: should some kind of meta magic
  shape->shape.add_tile( Tile{ { -1, 0 }, { 2, 0 } } );
  shape->shape.add_tile( Tile{ { 0, 1 }, { 0, 1 } } );
  shape->shape.add_tile( Tile{ { 0, -1 }, { 0, -1 } } );
  ship->add_behavior( ObjectBehavior::Pointer( shape ) );

  const Coordinate main_thruster_relative_to_center_of_mass(
      Coordinate{ -5_metres, Tile::unit_length / 2 } - shape->shape.center_of_mass() );

  ship->add_behavior( ObjectBehavior::Pointer( new Thruster(
          Command::main_thruster,
          main_thruster_relative_to_center_of_mass,
          180_degrees ) ) );

  const Coordinate front_thrusters_relative_to_center_of_mass(
      Coordinate{ 15_metres, Tile::unit_length / 2 } - shape->shape.center_of_mass() );

  ship->add_behavior( ObjectBehavior::Pointer( new Thruster(
          Command::port_thruster,
          front_thrusters_relative_to_center_of_mass,
          45_degrees ) ) );

  ship->add_behavior( ObjectBehavior::Pointer( new Thruster(
          Command::starboard_thruster,
          front_thrusters_relative_to_center_of_mass,
          -45_degrees ) ) );

  ship->add_behavior( ObjectBehavior::Pointer( new Canon() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::ship_layer ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 100 ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new LootDropper() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new RespawnWhenDestroyed() ) );

  ship->add_behavior( ObjectBehavior::Pointer( new ShapeGraphics() ) );
  return ship;
}

Object::Pointer
create_laser( const PhysicalParameters& laser_parameters )
{
  Object::Pointer laser( new Object() );
  std::unique_ptr< PhysicalBehavior > physical_behavior( new PhysicalBehavior( laser_parameters ) );
  physical_behavior->physical_parameters.angular_velocity = 0;
  physical_behavior->physical_parameters.velocity += heading( laser_parameters, laser_speed );
  physical_behavior->physical_parameters.coordinate += heading( laser_parameters, 10_metres );
  laser->add_behavior( ObjectBehavior::Pointer( physical_behavior.release() ) );
  laser->add_behavior( ObjectBehavior::Pointer( new LaserGraphics() ) );
  laser->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::laser_layer ) ) );
  laser->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 10 ) ) );
  laser->add_behavior( ObjectBehavior::Pointer( new DeleteWhenDestroyed() ) );
  laser->add_behavior( ObjectBehavior::Pointer( new SelfDestructor( laser->id, 3000000u ) ) );

  return laser;
}


}

