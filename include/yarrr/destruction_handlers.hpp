#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{

class DeleteWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_delete_when_destroyed" );
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;
};

}

