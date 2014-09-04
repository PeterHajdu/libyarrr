#pragma once

#include <yarrr/object.hpp>
#include <vector>
#include <functional>

namespace yarrr
{

class Inventory : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_inventory" );
    Inventory();
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;

    void register_item( ObjectBehavior& );

    typedef std::vector< std::reference_wrapper< ObjectBehavior > > ItemContainer;
    const ItemContainer& items() const;

  private:
    ItemContainer m_items;
};

}

