#include <yarrr/inventory.hpp>
#include <yarrr/entity_factory.hpp>

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

}

