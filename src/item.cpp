#include <yarrr/item.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/shape_behavior.hpp>

namespace yarrr
{

Item::Item( int synchronization_period, const std::string& name, const Tile::Coordinate& tile_coordinate )
  : ObjectBehavior( synchronization_period )
  , m_tile_coordinate( tile_coordinate )
  , m_shape( nullptr )
  , m_name( name )
{
}

Item::Item( int synchronization_period, const Id& id, const std::string& name, const Tile::Coordinate& tile_coordinate )
  : ObjectBehavior( synchronization_period, id )
  , m_tile_coordinate( tile_coordinate )
  , m_shape( nullptr )
  , m_name( name )
{
}

const Tile::Coordinate&
Item::tile_coordinate() const
{
  return m_tile_coordinate;
}

const Coordinate
Item::relative_coordinate() const
{
  return Coordinate{
      static_cast< int64_t >( ( 0.5 + m_tile_coordinate.x ) * yarrr::Tile::unit_length ),
      static_cast< int64_t >( ( 0.5 + m_tile_coordinate.y ) * yarrr::Tile::unit_length ) } -
      m_shape->center_of_mass();
}

void
Item::do_register_to( Object& owner )
{
  assert( owner.components.has_component< Inventory >() );
  owner.components.component< Inventory >().register_item( *this );

  assert( owner.components.has_component< ShapeBehavior >() );
  m_shape = &component_of< ShapeBehavior >( owner ).shape;

  register_item_to( owner );
}


const std::string&
Item::name() const
{
  return m_name;
}


}

