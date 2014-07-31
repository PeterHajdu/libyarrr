#pragma once

#include <yarrr/types.hpp>
#include <thectci/id.hpp>
#include <memory>

namespace yarrr
{
class Deserializer;
class Serializer;

class Entity
{
  public:
    typedef std::unique_ptr<Entity> Pointer;

    add_pure_polymorphic_ctci();
    virtual ~Entity() = default;

    Data serialize() const;
    void deserialize( const Data& data );

  private:
    virtual void do_serialize( Serializer& serializer ) const = 0;
    virtual void do_deserialize( Deserializer& deserializer ) = 0;
};

}

