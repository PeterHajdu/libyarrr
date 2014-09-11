#include <yarrr/object_creator.hpp>
#include <yarrr/object.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/engine.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/entity_factory.hpp>

#include <random>

namespace
{

class LootGraphics : public yarrr::GraphicalBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_loot_graphics" );
    LootGraphics() = default;
    LootGraphics( const yarrr::ObjectBehavior::Id& id )
      : GraphicalBehavior( id )
    {
    }

    virtual void draw() const
    {
    }

    yarrr::ObjectBehavior::Pointer clone() const
    {
      return yarrr::ObjectBehavior::Pointer( new LootGraphics( id() ) );
    }
};

yarrr::AutoEntityRegister< LootGraphics > auto_loot_graphics_register;

const int laser_speed{ 1000 };

std::random_device random_device;
std::default_random_engine random_engine( random_device() );
std::uniform_int_distribution<int> distribution{ -300, 300 };

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
  loot->add_behavior( ObjectBehavior::Pointer( new LootGraphics() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new DeleteWhenDestroyed() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 30 ) ) );
  loot->add_behavior( ObjectBehavior::Pointer( new LootAttacher() ) );
  loot->add_behavior( ObjectBehavior::Pointer( new SelfDestructor( loot->id, 36000000u ) ) );
  loot->add_behavior( item.clone() );

  return loot;
}


Object::Pointer
create_ship()
{
  Object::Pointer ship( new Object() );
  ship->add_behavior( ObjectBehavior::Pointer( new Inventory() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new PhysicalBehavior() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new LootDropper() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Engine() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Canon() ) );
  ship->add_behavior( ObjectBehavior::Pointer( new Collider( Collider::ship_layer ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new DamageCauser( 100 ) ) );
  ship->add_behavior( ObjectBehavior::Pointer( new RespawnWhenDestroyed() ) );

  ShapeBehavior* shape( new ShapeBehavior() );
  //todo: should some kind of meta magic
  shape->shape.add_tile( Tile{ { -1, 0 }, { 2, 0 } } );
  shape->shape.add_tile( Tile{ { 0, 1 }, { 0, 1 } } );
  shape->shape.add_tile( Tile{ { 0, -1 }, { 0, -1 } } );
  ship->add_behavior( ObjectBehavior::Pointer( shape ) );

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

