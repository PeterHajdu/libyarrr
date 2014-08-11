#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class ObjectDestroyed;

class DeleteWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_delete_when_destroyed" );
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;

  private:
    void handle_object_destroyed( const ObjectDestroyed& ) const;
    Object::Id m_object_id;
};

}

