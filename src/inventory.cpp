#include <yarrr/inventory.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <thectci/service_registry.hpp>

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
{
}

void
LootDropper::register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >(
      std::bind( &LootDropper::handle_object_destroyed, this, std::placeholders::_1 ) );
}

void
LootDropper::handle_object_destroyed( const ObjectDestroyed& ) const
{
  the::ctci::service< yarrr::EngineDispatcher >().dispatch(
      ObjectCreated( Object::Pointer( new Object() ) ) );
}

ObjectBehavior::Pointer
LootDropper::clone() const
{
  return Pointer( new LootDropper() );
}

}

