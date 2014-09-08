#include <yarrr/object_creator.hpp>
#include <yarrr/object.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/engine.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>

namespace
{

const int laser_speed{ 1000 };

}


namespace yarrr
{

Object::Pointer
create_ship()
{
  yarrr::Object::Pointer ship( new yarrr::Object() );
  ship->add_behavior( ObjectBehavior::Pointer( new Inventory() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new PhysicalBehavior() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new LootDropper() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Engine() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new ShipGraphics() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Canon() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::ship_layer ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 100 ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new RespawnWhenDestroyed() ) );
  return ship;
}

Object::Pointer
create_laser( const PhysicalParameters& laser_parameters )
{
  Object::Pointer laser( new Object() );
  std::unique_ptr< PhysicalBehavior > physical_behavior( new PhysicalBehavior( laser_parameters ) );
  physical_behavior->physical_parameters.vangle = 0;
  physical_behavior->physical_parameters.velocity += heading( laser_parameters, laser_speed );
  physical_behavior->physical_parameters.coordinate += heading( laser_parameters, 100 );
  laser->add_behavior( ObjectBehavior::Pointer( physical_behavior.release() ) );
  laser->add_behavior( ObjectBehavior::Pointer( new LaserGraphics() ) );
  laser->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::laser_layer ) ) );
  laser->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 10 ) ) );
  laser->add_behavior( ObjectBehavior::Pointer( new DeleteWhenDestroyed() ) );
  laser->add_behavior( ObjectBehavior::Pointer( new SelfDestructor( laser->id, 3000000u ) ) );

  return laser;
}


}

