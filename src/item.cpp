#include <yarrr/item.hpp>
#include <yarrr/inventory.hpp>

namespace yarrr
{

Item::Item( const std::string& name )
  : ObjectBehavior( synchronize )
  , m_name( name )
{
}

Item::Item( const Id& id, const std::string& name )
  : ObjectBehavior( synchronize, id )
  , m_name( name )
{
}


void
Item::do_register_to( Object& owner )
{
  assert( owner.components.has_component< Inventory >() );
  owner.components.component< Inventory >().register_item( *this );
  register_item_to( owner );
}


const std::string&
Item::name() const
{
  return m_name;
}


}

