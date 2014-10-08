#pragma once

#include <yarrr/object.hpp>
#include <yarrr/shape.hpp>

namespace yarrr
{
class Serializer;
class Deserializer;

class Item : public ObjectBehavior
{
  public:
    Item( int synchronization_period, const std::string& name, const Tile::Coordinate& );
    Item( int synchronization_period, const Id& id, const std::string& name, const Tile::Coordinate& );
    virtual ~Item() = default;
    const std::string& name() const;

    const Tile::Coordinate& tile_coordinate() const;
    const Coordinate relative_coordinate() const;
  private:
    virtual void do_register_to( Object& ) override final;
    virtual void register_item_to( Object& ) {};
    virtual void serialize_behavior( Serializer& serializer ) const override final;
    virtual void deserialize_behavior( Deserializer& deserializer ) override final;

    virtual void serialize_item( Serializer& serializer ) const {};
    virtual void deserialize_item( Deserializer& deserializer ) {};

    Tile::Coordinate m_tile_coordinate;
    Shape* m_shape;
    const std::string m_name;
};

}

