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
    typedef uint32_t Id;
    add_polymorphic_ctci( "command" );

    Command() = default;
    Command( Id id, the::time::Time timestamp );
    Id id() const;
    const the::time::Time& timestamp() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    Id m_id;
    the::time::Time m_timestamp;
};

}

