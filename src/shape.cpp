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
    ( bottom_right.x + top_left.x + 1 ) * 0.5 * unit_length,
    ( top_left.y + bottom_right.y + 1 ) * 0.5 * unit_length };
}

void
Shape::add_tile( const Tile& tile )
{
  m_tiles.emplace_back( tile );
  calculate_center_of_mass();
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
{
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
Shape::calculate_center_of_mass()
{
  size_t running_mass{ 0 };
  for ( const auto& tile : m_tiles )
  {
    const size_t new_mass{ running_mass + tile.mass };
    m_center_of_mass = ( m_center_of_mass * running_mass + tile.center * tile.mass ) * ( 1.0 / new_mass );
    running_mass = new_mass;
  }
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
  const double angle_in_rad( hiplon_to_radians( angle ) );
  const double cos_angle( cos( angle_in_rad ) );
  const double sin_angle( sin( angle_in_rad ) );
  for ( auto& point : polygon )
  {
    Coordinate new_coordinate{
      point.x * cos_angle - point.y * sin_angle,
      point.x * sin_angle + point.y * cos_angle };
    point = new_coordinate;
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

}

