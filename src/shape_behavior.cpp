#include <yarrr/shape_behavior.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>

namespace
{

  yarrr::AutoEntityRegister< yarrr::ShapeBehavior > auto_shape_register;

}

namespace yarrr
{

ShapeBehavior::ShapeBehavior()
  : ObjectBehavior( synchronize )
{
}

ShapeBehavior::ShapeBehavior( const Id& id )
  : ObjectBehavior( synchronize, id )
{
}


ObjectBehavior::Pointer
ShapeBehavior::clone() const
{
  return Pointer( new ShapeBehavior( id() ) );
}

void
ShapeBehavior::serialize_behavior( Serializer& serializer ) const
{
  const Shape::TileContainer& tiles( shape.tiles() );
  const uint16_t number_of_tiles( tiles.size() );
  serializer.push_back( number_of_tiles );

  for ( size_t i( 0 ); i < number_of_tiles; ++i )
  {
    serialize_tile( serializer, tiles.at( i ) );
  }

}


void
ShapeBehavior::serialize_tile( Serializer& serializer, const Tile& tile ) const
{
  serializer.push_back( tile.top_left.x );
  serializer.push_back( tile.top_left.y );
  serializer.push_back( tile.bottom_right.x );
  serializer.push_back( tile.bottom_right.y );
}


void
ShapeBehavior::deserialize_behavior( Deserializer& deserializer )
{
  const uint16_t number_of_tiles( deserializer.pop_front< uint16_t >() );

  for ( size_t i( 0 ); i < number_of_tiles; ++i )
  {
    shape.add_tile( deserialize_tile( deserializer ) );
  }
}

Tile
ShapeBehavior::deserialize_tile( Deserializer& deserializer ) const
{
  int16_t x;
  int16_t y;
  x = deserializer.pop_front< int16_t >();
  y = deserializer.pop_front< int16_t >();

  const Tile::Coordinate top_left( x, y );

  x = deserializer.pop_front< int16_t >();
  y = deserializer.pop_front< int16_t >();

  const Tile::Coordinate bottom_right( x, y );

  return Tile{ top_left, bottom_right };
}

Tile::Coordinate
ShapeBehavior::deserialize_coordinate( Deserializer& deserializer ) const
{
  const int16_t x( deserializer.pop_front< int16_t >() );
  const int16_t y( deserializer.pop_front< int16_t >() );

  return Tile::Coordinate{ x, y };
}

}

