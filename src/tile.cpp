#include <yarrr/tile.hpp>

namespace
{
  yarrr::Tile::Coordinate top_left_from(
      const yarrr::Tile::Coordinate& a,
      const yarrr::Tile::Coordinate& b )
  {
    return { std::min( a.x, b.x ), std::max( a.y, b.y ) };
  }

  yarrr::Tile::Coordinate bottom_right_from(
      const yarrr::Tile::Coordinate& a,
      const yarrr::Tile::Coordinate& b )
  {
    return { std::max( a.x, b.x ), std::min( a.y, b.y ) };
  }

  yarrr::Coordinate::type
  left_side_of( const yarrr::Tile& tile )
  {
    return tile.top_left.x * yarrr::Tile::unit_length;
  }

  yarrr::Coordinate::type
  right_side_of( const yarrr::Tile& tile )
  {
    return ( tile.bottom_right.x + 1 ) * yarrr::Tile::unit_length;
  }

  yarrr::Coordinate::type
  top_side_of( const yarrr::Tile& tile )
  {
    return ( tile.top_left.y + 1 ) * yarrr::Tile::unit_length;
  }

  yarrr::Coordinate::type
  bottom_side_of( const yarrr::Tile& tile )
  {
    return tile.bottom_right.y * yarrr::Tile::unit_length;
  }
}

namespace yarrr
{

const int16_t Tile::unit_length = 5_metres;

Tile::Tile( const Coordinate& top_left, const Coordinate& bottom_right )
  : top_left( top_left_from( top_left, bottom_right ) )
  , bottom_right( bottom_right_from( top_left, bottom_right ) )
  , center( calculate_center() )
  , mass( calculate_mass() )
{
}

bool
Tile::does_contain( const yarrr::Coordinate& coordinate ) const
{
  return
    coordinate.x > left_side_of( *this ) &&
    coordinate.y < top_side_of( *this ) &&
    coordinate.x < right_side_of( *this ) &&
    coordinate.y > bottom_side_of( *this );
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
  return ( top_left_corner_of( *this ) + bottom_right_corner_of( *this ) ) * 0.5;

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

Coordinate
top_left_corner_of( const Tile& tile )
{
  return yarrr::Coordinate{ left_side_of( tile ), top_side_of( tile ) };
}

Coordinate
top_right_corner_of( const Tile& tile )
{
  return yarrr::Coordinate{ right_side_of( tile ), top_side_of( tile ) };
}

Coordinate
bottom_right_corner_of( const Tile& tile )
{
  return yarrr::Coordinate{ right_side_of( tile ), bottom_side_of( tile ) };
}

Coordinate
bottom_left_corner_of( const Tile& tile )
{
  return yarrr::Coordinate{ left_side_of( tile ), bottom_side_of( tile ) };
}

}

