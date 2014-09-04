#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{

class Inventory : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_inventory" );
    Inventory();
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;
};

}

