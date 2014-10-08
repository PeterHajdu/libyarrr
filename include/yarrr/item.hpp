#pragma once

#include <yarrr/object.hpp>
#include <yarrr/shape.hpp>

namespace yarrr
{

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

    Tile::Coordinate m_tile_coordinate;
    Shape* m_shape;
    const std::string m_name;
};

}

