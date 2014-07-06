#pragma once

#include <yarrr/event.hpp>
#include <yarrr/types.hpp>
#include <thetime/clock.hpp>
#include <cstdint>

namespace yarrr
{

class Command : public Event
{
  public:
    typedef uint32_t Type;
    enum : Type
    {
      thruster = 1,
      cw = 2,
      ccw = 3
    };

    add_polymorphic_ctci( "command" );

    Command() = default;
    Command( Type type, the::time::Time timestamp );
    Type type() const;
    const the::time::Time& timestamp() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    Type m_type;
    the::time::Time m_timestamp;
};

}

