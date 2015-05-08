#include <yarrr/object_creator.hpp>
#include <yarrr/object.hpp>
#include <yarrr/object_identity.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/cargo.hpp>
#include <yarrr/loot.hpp>
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
create_loot(
    PhysicalParameters new_physical_parameters,
    const Tile& tile,
    const std::vector< Goods >& goods )
{
  add_random_parameters( new_physical_parameters );
  auto loot( std::make_unique< Object >() );
  auto cargo_space( std::make_unique< CargoSpace >() );
  for ( const auto& good : goods )
  {
    cargo_space->add_goods( good );
  }
  loot->add_behavior( std::move( cargo_space ) );

  loot->add_behavior( std::make_unique< PhysicalBehavior >( new_physical_parameters ) );
  loot->add_behavior( std::make_unique< Collider >( Collider::loot_layer ) );
  loot->add_behavior( std::make_unique< LootAttacher >() );
  loot->add_behavior( std::make_unique< SelfDestructor >( loot->id(), 36000000u ) );
  loot->add_behavior( delete_when_destroyed() );

  auto shape( std::make_unique< ShapeBehavior >() );
  shape->shape.add_tile( tile );
  loot->add_behavior( std::move( shape ) );
  loot->add_behavior( std::make_unique< ShapeGraphics >() );

  loot->add_behavior( std::make_unique< DamageCauser >() );


  return loot;
}



Object::Pointer
create_laser( const PhysicalParameters& laser_parameters, const ObjectIdentity& identity )
{
  auto laser( std::make_unique< Object >() );
  std::unique_ptr< PhysicalBehavior > physical_behavior( new PhysicalBehavior( laser_parameters ) );
  physical_behavior->physical_parameters.angular_velocity = 0;
  physical_behavior->physical_parameters.velocity += heading( laser_parameters, laser_speed );
  //todo: remove after solving suicide issue
  physical_behavior->physical_parameters.coordinate += heading( laser_parameters, 10_metres );
  laser->add_behavior( ObjectBehavior::Pointer( physical_behavior.release() ) );

  laser->add_behavior( std::make_unique< LaserGraphics >() );
  laser->add_behavior( std::make_unique< Collider >( Collider::laser_layer ) );
  laser->add_behavior( std::make_unique< DamageCauser >() );
  laser->add_behavior( std::make_unique< SelfDestructor >( laser->id(), 3000000u ) );
  laser->add_behavior( std::make_unique< ObjectIdentity >( identity.captain() ) );
  laser->add_behavior( delete_when_destroyed() );

  return laser;
}


}

namespace test
{

yarrr::Object::Pointer
create_ship()
{
  using namespace yarrr;
  auto ship( std::make_unique< Object >() );
  ship->add_behavior( std::make_unique< Inventory >() );
  ship->add_behavior( std::make_unique< CargoSpace >() );
  ship->add_behavior( std::make_unique< PhysicalBehavior >() );

  auto shape( std::make_unique< ShapeBehavior >() );
  shape->shape.add_tile( Tile{ { -1, 0 }, { 2, 0 } } );
  shape->shape.add_tile( Tile{ { 0, 1 }, { 0, 1 } } );
  shape->shape.add_tile( Tile{ { 0, -1 }, { 0, -1 } } );
  ship->add_behavior( std::move( shape ) );

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
  ship->add_behavior( ObjectBehavior::Pointer( new DamageCauser() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new LootDropper() ) );
  ship->add_behavior( kill_player_when_destroyed() );

  ship->add_behavior( ObjectBehavior::Pointer( new ShapeGraphics() ) );
  return ship;
}

}

