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

    const Tile::Coordinate tile_coordinate;
  private:
    virtual void do_register_to( Object& ) override final;
    virtual void register_item_to( Object& ) {};

    const std::string m_name;
};

}

