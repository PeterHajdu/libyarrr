#pragma once

#include <yarrr/entity.hpp>
#include <yarrr/types.hpp>
#include <thetime/clock.hpp>
#include <cstdint>

namespace yarrr
{

class ShipControl : public Entity
{
  public:
    typedef uint32_t Type;
    enum : Type
    {
      main_thruster = 1,
      port_thruster = 2,
      starboard_thruster = 3,
      fire = 4
    };

    add_polymorphic_ctci( "yarrr_ship_control" );

    ShipControl() = default;
    ShipControl( Type type, the::time::Time timestamp );
    Type type() const;
    const the::time::Time& timestamp() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    Type m_type;
    the::time::Time m_timestamp;
};

class Fire
{
  public:
    add_ctci( "yarrr_fire" );
    Fire( const Angle& angle )
      : direction( angle )
    {
    }

    const Angle direction;
};

}

