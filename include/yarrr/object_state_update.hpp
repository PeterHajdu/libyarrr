#pragma once

#include <yarrr/event.hpp>
#include <yarrr/physical_parameters.hpp>

namespace yarrr
{
class PhysicalParameters;

class ObjectStateUpdate : public Event
{
  public:
    add_polymorphic_ctci( "object_state_update" );

    ObjectStateUpdate() = default;
    ObjectStateUpdate( const PhysicalParameters& );
    const PhysicalParameters& physical_parameters() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    PhysicalParameters m_physical_parameters;
};

}

