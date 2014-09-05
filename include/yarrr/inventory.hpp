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


class ObjectDestroyed;
class PhysicalParameters;
class Inventory;
class LootDropper : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_loot_dropper" );
    LootDropper();
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;

  private:
    void handle_object_destroyed( const ObjectDestroyed& ) const;
    PhysicalParameters* m_owner_parameters;
    Inventory* m_inventory;
};

}

