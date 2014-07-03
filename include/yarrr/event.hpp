#pragma once

#include <yarrr/types.hpp>
#include <thectci/id.hpp>
#include <memory>

namespace yarrr
{
class Deserializer;
class Serializer;

class Event
{
  public:
    typedef std::unique_ptr<Event> Pointer;

    add_pure_polymorphic_ctci();
    virtual ~Event() = default;

    Data serialize() const;
    void deserialize( const Data& data );

  private:
    virtual void do_serialize( Serializer& serializer ) const = 0;
    virtual void do_deserialize( Deserializer& deserializer ) = 0;
};

}

