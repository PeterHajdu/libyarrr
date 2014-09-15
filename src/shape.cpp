#include <yarrr/shape.hpp>

namespace yarrr
{

const int16_t Tile::unit_length = 5_metres;

Tile::Tile( const Coordinate& top_left, const Coordinate& bottom_right )
  : top_left( top_left )
  , bottom_right( bottom_right )
  , center( calculate_center() )
  , mass( calculate_mass() )
{
}

int
Tile::calculate_mass() const
{
  return
    ( bottom_right.x-top_left.x + 1 ) *
    ( top_left.y - bottom_right.y + 1 );
}

yarrr::Coordinate
Tile::calculate_center() const
{
  return {
    static_cast< int64_t >( ( bottom_right.x + top_left.x + 1 ) * 0.5 * unit_length ),
    static_cast< int64_t >( ( top_left.y + bottom_right.y + 1 ) * 0.5 * unit_length ) };
}

void
Shape::add_tile( const Tile& tile )
{
  m_tiles.emplace_back( tile );
  calculate_center_of_mass_and_mass();
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
{
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

}

