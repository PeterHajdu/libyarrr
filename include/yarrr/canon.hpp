#pragma once

#include <yarrr/types.hpp>
#include <yarrr/item.hpp>

namespace yarrr
{

class ShipControl;
class PhysicalParameters;
class Canon : public Item
{
  public:
    add_polymorphic_ctci( "yarrr_canon" );
    Canon();
    Canon( const Tile::Coordinate coordinate, const Angle& orientation );
    Canon( const Id& id, const Tile::Coordinate coordinate, const Angle& orientation );

    virtual Pointer clone() const override;

    Angle orientation() const;

  private:
    virtual void serialize_item( Serializer& serializer ) const override final;
    virtual void deserialize_item( Deserializer& deserializer ) override final;

    virtual void register_item_to( Object& ) override;
    void handle_command( const ShipControl& ) const;
    PhysicalParameters generate_physical_parameters() const;

    PhysicalParameters* m_physical_parameters;
    Angle m_orientation;
};

}

