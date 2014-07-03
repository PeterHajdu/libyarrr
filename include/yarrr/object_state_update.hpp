#pragma once

#include <yarrr/event.hpp>
#include <yarrr/object.hpp>

namespace yarrr
{
class Object;

class ObjectStateUpdate : public Event
{
  public:
    add_polymorphic_ctci( "object_state_update" );

    ObjectStateUpdate() = default;
    ObjectStateUpdate( const Object& );
    const Object& object() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    Object m_object;
};

}

