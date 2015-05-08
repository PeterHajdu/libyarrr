#pragma once

#include <yarrr/object.hpp>
#include <yarrr/item.hpp>
#include <vector>

namespace yarrr
{

class Inventory : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_inventory" );
    Inventory();
    Inventory( const Id& id );
    virtual Pointer clone() const override;

    void register_item( Item& );

    typedef std::vector< std::reference_wrapper< Item > > ItemContainer;
    const ItemContainer& items() const;

  private:
    ItemContainer m_items;
};

}

