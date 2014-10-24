#include <yarrr/shape.hpp>

namespace
{
  typedef std::vector< yarrr::Coordinate > Corners;

  //todo: extract these bound calculations
  Corners corners_of( const yarrr::Tile& tile )
  {
    return {
      yarrr::Coordinate( tile.top_left.x, tile.top_left.y + 1 ) * yarrr::Tile::unit_length,
      yarrr::Coordinate( tile.bottom_right.x + 1, tile.top_left.y + 1 ) * yarrr::Tile::unit_length,
      yarrr::Coordinate( tile.bottom_right.x + 1, tile.bottom_right.y ) * yarrr::Tile::unit_length,
      yarrr::Coordinate( tile.top_left.x, tile.bottom_right.y ) * yarrr::Tile::unit_length,
      };
  }
}

namespace yarrr
{

void
Shape::add_tile( const Tile& tile )
{
  m_tiles.emplace_back( tile );
  calculate_center_of_mass_and_mass();
  calculate_radius();
}

bool
operator==( const Tile& l, const Tile& r )
{
  return
    l.top_left == r.top_left &&
    l.bottom_right == r.bottom_right;
}

bool
operator!=( const Tile& l, const Tile& r )
{
  return !( l == r );
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

Polygon
shape_coordinate_polygon_from( const Tile& tile )
{
  const int left{ tile.top_left.x };
  const int right{ tile.bottom_right.x + 1 };
  const int top{ tile.top_left.y + 1 };
  const int bottom{ tile.bottom_right.y };
  return Polygon{
      Coordinate{ left, top } * Tile::unit_length,
      Coordinate{ right, top } * Tile::unit_length,
      Coordinate{ right, bottom } * Tile::unit_length,
      Coordinate{ left, bottom } * Tile::unit_length };
}

void
transform_coordinates_to_new_origo( Polygon& polygon , const Coordinate& origo )
{
  for ( auto& point : polygon )
  {
    point.x-=origo.x;
    point.y-=origo.y;
  }
}

void
rotate_with( Polygon& polygon, Angle angle )
{
  for ( auto& point : polygon )
  {
    rotate( point, angle );
  }
}

Polygon
generate_polygon_from(
    const Tile& tile,
    const Coordinate& center_of_object,
    const Coordinate& center_of_mass_of_shape,
    const Angle& orientation_of_object )
{
  Polygon polygon( shape_coordinate_polygon_from( tile ) );
  transform_coordinates_to_new_origo( polygon, center_of_mass_of_shape );
  rotate_with( polygon, orientation_of_object );
  transform_coordinates_to_new_origo( polygon, center_of_object * -1 );
  return polygon;
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

Coordinate
relative_to_absolute(
    const Coordinate& relative_coordinate,
    const Coordinate& center_of_mass_relative,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object )
{
  return center_of_mass_relative_to_absolute(
      relative_coordinate - center_of_mass_relative,
      center_of_object_absolute,
      orientation_of_object );
}

Coordinate
center_of_mass_relative_to_absolute(
    const Coordinate& relative_to_center_of_mass_coordinate,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object )
{
  Coordinate absolute_coordinate( relative_to_center_of_mass_coordinate );
  rotate( absolute_coordinate, orientation_of_object );
  absolute_coordinate += center_of_object_absolute;
  return absolute_coordinate;
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
    const Corners corners( corners_of( tile ) );
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


Coordinate
transform_absolute_to_relative_to_center_of_mass(
  const Coordinate& absolute_coordinate,
  const Coordinate& absolute_center_of_mass,
  const Angle& orientation )
{
  Coordinate relative( absolute_coordinate - absolute_center_of_mass );
  yarrr::rotate( relative, -orientation );
  return relative;
}


}

