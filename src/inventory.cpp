#include <yarrr/inventory.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <thectci/service_registry.hpp>

namespace
{

yarrr::Object::Pointer create_loot_object()
{
  yarrr::Object::Pointer loot_object( new yarrr::Object() );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Inventory() ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );
  loot_object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::Collider( yarrr::Collider::loot_layer ) ) );
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
      ObjectCreated( create_loot_object() ) );
}

ObjectBehavior::Pointer
LootDropper::clone() const
{
  return Pointer( new LootDropper() );
}

}

