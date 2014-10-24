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
//todo: extract these bound calculations
  return
    coordinate.x > top_left.x * unit_length &&
    coordinate.y < ( top_left.y + 1 ) * unit_length &&
    coordinate.x < ( bottom_right.x + 1 ) * unit_length &&
    coordinate.y > bottom_right.y * unit_length;
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

}

