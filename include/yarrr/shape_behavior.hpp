#pragma once

#include <yarrr/shape.hpp>
#include <yarrr/object.hpp>
#include <thectci/id.hpp>

namespace yarrr
{
class Serializer;
class Deserializer;

class ShapeBehavior : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_shape_vehavior" );
    ShapeBehavior();
    ShapeBehavior( const Shape& );
    ShapeBehavior( const Id& id );
    virtual Pointer clone() const override;

    Shape shape;

  private:
    virtual void do_register_to( Object& ) override;

    virtual void serialize_behavior( Serializer& serializer ) const override;
    virtual void deserialize_behavior( Deserializer& deserializer ) override;

    void serialize_tile( Serializer&, const Tile& ) const;
    Tile deserialize_tile( Deserializer& ) const;
    Tile::Coordinate deserialize_coordinate( Deserializer& ) const;
};

}

