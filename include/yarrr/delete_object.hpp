#pragma once

#include <yarrr/event.hpp>
#include <thectci/id.hpp>
#include <yarrr/object.hpp>

namespace yarrr
{

class DeleteObject : public yarrr::Event
{
  public:
    add_polymorphic_ctci( "delete_object" );
    DeleteObject() = default;
    DeleteObject( const Object::Id& id );
    const Object::Id& object_id() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    Object::Id m_object_id;
};

}

