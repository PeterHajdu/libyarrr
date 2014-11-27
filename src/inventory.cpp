#include <yarrr/inventory.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/object_creator.hpp>
#include <yarrr/object_created.hpp>
#include <thectci/service_registry.hpp>

namespace
{

yarrr::AutoEntityRegister< yarrr::Inventory > auto_inventory_register;

}

namespace yarrr
{

Inventory::Inventory()
  : ObjectBehavior( rarely_synchronize() )
{
}

Inventory::Inventory( const Id& id )
  : ObjectBehavior( rarely_synchronize(), id )
{
}

ObjectBehavior::Pointer
Inventory::clone() const
{
  return Pointer( new Inventory( id() ) );
}

void
Inventory::do_register_to( Object& )
{
}

void
Inventory::register_item( Item& item )
{
  m_items.emplace_back( item );
  force_synchronization();
}

const Inventory::ItemContainer&
Inventory::items() const
{
  return m_items;
}

LootDropper::LootDropper()
  : ObjectBehavior( do_not_synchronize() )
  , m_owner_parameters( nullptr )
  , m_inventory( nullptr )
{
}

void
LootDropper::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >(
      [ this ]( const ObjectDestroyed& destroyed ){ handle_object_destroyed( destroyed ); } );
  m_owner_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
  m_inventory = &owner.components.component< Inventory >();
}

void
LootDropper::handle_object_destroyed( const ObjectDestroyed& ) const
{
  for ( const auto& item : m_inventory->items() )
  {
    the::ctci::service< yarrr::EngineDispatcher >().dispatch(
        ObjectCreated( create_loot( *m_owner_parameters, item.get() ) ) );
  }
}

ObjectBehavior::Pointer
LootDropper::clone() const
{
  return Pointer( new LootDropper() );
}

LootAttacher::LootAttacher()
  : ObjectBehavior( do_not_synchronize() )
  , m_inventory( nullptr )
{
}

void
LootAttacher::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::Collide >(
      [ this ]( const Collide& collide ){ handle_object_collided( collide ); } );
  m_inventory = &owner.components.component< Inventory >();
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
  m_object->dispatcher.dispatch( ObjectDestroyed() );
}

ObjectBehavior::Pointer
LootAttacher::clone() const
{
  return Pointer( new LootAttacher() );
}
}

