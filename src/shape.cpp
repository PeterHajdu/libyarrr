#include <yarrr/shape.hpp>
#include <yarrr/polygon.hpp>
#include <algorithm>

namespace yarrr
{

void
Shape::add_tile( const Tile& tile )
{
  m_tiles.emplace_back( tile );
  calculate_center_of_mass_and_mass();
  calculate_radius();
}


Shape::Shape()
  : m_center_of_mass{ 0, 0 }
  , m_mass{ 0 }
  , m_radius{ 0 }
{
}

Shape::Shape( const TileContainer& tiles )
  : Shape()
{
  for ( const auto& tile : tiles )
  {
    add_tile( tile );
  }
}

int
Shape::mass() const
{
  return m_mass;
}

const Shape::TileContainer&
Shape::tiles() const
{
  return m_tiles;
}


bool operator==( const Shape& l, const Shape& r )
{
  const Shape::TileContainer& l_tiles( l.tiles() );
  const Shape::TileContainer& r_tiles( r.tiles() );
  if ( l_tiles.size() != r_tiles.size() )
  {
    return false;
  }

  for ( size_t i( 0 ); i < l_tiles.size(); ++i )
  {
    if ( l_tiles.at( i ) != r_tiles.at( i ) )
    {
      return false;
    }
  }

  return true;
}


const Coordinate&
Shape::center_of_mass() const
{
  return m_center_of_mass;
}


void
Shape::calculate_center_of_mass_and_mass()
{
  int running_mass{ 0 };
  for ( const auto& tile : m_tiles )
  {
    const int new_mass{ running_mass + tile.mass };
    m_center_of_mass = ( m_center_of_mass * running_mass + tile.center * tile.mass ) * ( 1.0 / new_mass );
    running_mass = new_mass;
  }

  m_mass = running_mass;
}


Shape&
Shape::operator=( const Shape& other )
{
  for ( const auto& tile : other.m_tiles )
  {
    add_tile( tile );
  }
  return *this;
}


Coordinate::type
Shape::radius() const
{
  return m_radius;
}

void
Shape::calculate_radius()
{
  m_radius = 0;

  for ( const auto& tile : m_tiles )
  {
    const Polygon corners( shape_coordinate_polygon_from( tile ) );
    for ( const auto& corner : corners )
    {
      const auto distance_from_center_of_mass( yarrr::length_of( m_center_of_mass - corner ) );
      m_radius = std::max( m_radius, distance_from_center_of_mass );
    }
  }

}


bool
Shape::does_contain( const Coordinate& relative_to_center_of_mass ) const
{
  const Coordinate relative_coordinate{ relative_to_center_of_mass + center_of_mass() };

  return std::any_of( std::begin( m_tiles ), std::end( m_tiles ),
     [ &relative_coordinate ]( const Tile& tile )
     {
       return tile.does_contain( relative_coordinate );
     } );
}

}

