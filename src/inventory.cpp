#include <yarrr/inventory.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/service_registry.hpp>

namespace
{

class LootGraphics : public yarrr::GraphicalBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_loot_graphics" );

    virtual void draw() const
    {
      m_graphical_engine.draw_loot( m_physical_behavior->physical_parameters );
    }

    yarrr::ObjectBehavior::Pointer clone() const
    {
      return yarrr::ObjectBehavior::Pointer( new LootGraphics() );
    }

  private:
    void do_register_to( yarrr::Object& ) override {}
};

yarrr::AutoEntityRegister< LootGraphics > auto_loot_graphics_register;

yarrr::Object::Pointer create_loot_object(
    const yarrr::PhysicalParameters& owner_parameters,
    const yarrr::Inventory::ItemContainer& items )
{
  yarrr::Object::Pointer loot_object( new yarrr::Object() );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior( owner_parameters ) ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Collider( yarrr::Collider::loot_layer ) ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new LootGraphics() ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::DeleteWhenDestroyed() ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::DamageCauser( 30 ) ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::LootAttacher() ) );

  for ( const auto& item : items )
  {
    loot_object->add_behavior( item.get().clone() );
  }

  return loot_object;
}

}

namespace yarrr
{

Inventory::Inventory()
  : ObjectBehavior( do_not_syncronize )
{
}

void
Inventory::register_to( Object& owner )
{
  owner.components.register_component( *this );
}

ObjectBehavior::Pointer
Inventory::clone() const
{
  return Pointer( new Inventory() );
}


void
Inventory::register_item( ObjectBehavior& item )
{
  m_items.emplace_back( item );
}

const Inventory::ItemContainer&
Inventory::items() const
{
  return m_items;
}

LootDropper::LootDropper()
  : ObjectBehavior( do_not_syncronize )
  , m_owner_parameters( nullptr )
  , m_inventory( nullptr )
{
}

void
LootDropper::register_to( Object& owner )
{
  owner.components.register_component( *this );
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >(
      std::bind( &LootDropper::handle_object_destroyed, this, std::placeholders::_1 ) );
  m_owner_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
  m_inventory = &owner.components.component< Inventory >();
}

void
LootDropper::handle_object_destroyed( const ObjectDestroyed& ) const
{
  the::ctci::service< yarrr::EngineDispatcher >().dispatch(
      ObjectCreated( create_loot_object( *m_owner_parameters, m_inventory->items() ) ) );
}

ObjectBehavior::Pointer
LootDropper::clone() const
{
  return Pointer( new LootDropper() );
}

LootAttacher::LootAttacher()
  : ObjectBehavior( do_not_syncronize )
  , m_inventory( nullptr )
  , m_dispatcher( nullptr )
{
}

void
LootAttacher::register_to( Object& owner )
{
  owner.components.register_component( *this );
  owner.dispatcher.register_listener< yarrr::Collide >(
      std::bind( &LootAttacher::handle_object_collided, this, std::placeholders::_1 ) );
  m_inventory = &owner.components.component< Inventory >();
  m_dispatcher = &owner.dispatcher;
}

void
LootAttacher::handle_object_collided( const Collide& collide ) const
{
  if ( !collide.with.components.has_component< Inventory >() )
  {
    return;
  }
  attach_items_to( collide.with );
}

void
LootAttacher::attach_items_to( Object& new_owner ) const
{
  for ( const auto& item : m_inventory->items() )
  {
    new_owner.add_behavior( item.get().clone() );
  }
  m_dispatcher->dispatch( ObjectDestroyed() );
}

ObjectBehavior::Pointer
LootAttacher::clone() const
{
  return Pointer( new LootAttacher() );
}
}

