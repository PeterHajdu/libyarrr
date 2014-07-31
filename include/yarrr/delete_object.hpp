#pragma once

#include <yarrr/entity.hpp>
#include <thectci/id.hpp>
#include <yarrr/physical_parameters.hpp>

namespace yarrr
{

class DeleteObject : public yarrr::Entity
{
  public:
    add_polymorphic_ctci( "delete_object" );
    DeleteObject() = default;
    DeleteObject( const PhysicalParameters::Id& id );
    const PhysicalParameters::Id& object_id() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    PhysicalParameters::Id m_object_id;
};

}

