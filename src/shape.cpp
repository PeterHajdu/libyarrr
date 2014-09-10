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

const Shape::TileContainer&
Shape::tiles() const
{
  return m_tiles;
}


}

