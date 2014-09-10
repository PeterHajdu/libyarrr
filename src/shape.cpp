#include <yarrr/shape.hpp>

namespace yarrr
{

void
Shape::add_tile( const Tile& tile )
{
  m_tiles.emplace_back( tile );
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

}

