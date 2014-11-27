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

const int laser_speed{ 200_metres };

std::random_device random_device;
std::default_random_engine random_engine( random_device() );
std::uniform_int_distribution<int> velocity_distribution{ -75_metres, 75_metres };
std::uniform_int_distribution<int> angular_velocity_distribution{ -500_degrees, +500_degrees };

void add_random_parameters( yarrr::PhysicalParameters& physical_parameters )
{
  physical_parameters.velocity += yarrr::Coordinate(
      velocity_distribution( random_engine ),
      velocity_distribution( random_engine ) );

  physical_parameters.angular_velocity +=
    angular_velocity_distribution( random_engine );
}

}


namespace yarrr
{

Object::Pointer
create_loot( PhysicalParameters new_physical_parameters, const ObjectBehavior& item )
{
  add_random_parameters( new_physical_parameters );
  Object::Pointer loot( new Object() );
  loot->add_behavior( ObjectBehavior::Pointer( new Inventory() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new PhysicalBehavior( new_physical_parameters ) ) );
  loot->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::loot_layer ) ) );
  loot->add_behavior( delete_when_destroyed() );
  loot->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 30 ) ) );
  loot->add_behavior( ObjectBehavior::Pointer( new LootAttacher() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new SelfDestructor( loot->id(), 36000000u ) ) );

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
  shape->shape.add_tile( Tile{ { -1, 0 }, { 2, 0 } } );
  shape->shape.add_tile( Tile{ { 0, 1 }, { 0, 1 } } );
  shape->shape.add_tile( Tile{ { 0, -1 }, { 0, -1 } } );
  ship->add_behavior( ObjectBehavior::Pointer( shape ) );

  ship->add_behavior( ObjectBehavior::Pointer( new Thruster(
          ShipControl::main_thruster,
          { -1, 0 },
          180_degrees ) ) );

  ship->add_behavior( ObjectBehavior::Pointer( new Thruster(
          ShipControl::port_thruster,
          { 2, 0 },
          90_degrees ) ) );

  ship->add_behavior( ObjectBehavior::Pointer( new Thruster(
          ShipControl::starboard_thruster,
          { 2, 0 },
          -90_degrees ) ) );

  ship->add_behavior( ObjectBehavior::Pointer( new Canon() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::ship_layer ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 100 ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new LootDropper() ) );
  ship->add_behavior( kill_player_when_destroyed() );

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
  laser->add_behavior( delete_when_destroyed() );
  laser->add_behavior( ObjectBehavior::Pointer( new SelfDestructor( laser->id(), 3000000u ) ) );

  return laser;
}


}

