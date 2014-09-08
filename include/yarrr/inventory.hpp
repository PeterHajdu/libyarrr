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
    virtual Pointer clone() const override;

    void register_item( ObjectBehavior& );

    typedef std::vector< std::reference_wrapper< ObjectBehavior > > ItemContainer;
    const ItemContainer& items() const;

  private:
    virtual void do_register_to( Object& ) override;
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
    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;
    void handle_object_destroyed( const ObjectDestroyed& ) const;
    PhysicalParameters* m_owner_parameters;
    Inventory* m_inventory;
};

class Collide;
class LootAttacher : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_loot_attacher" );
    LootAttacher();
    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;
    void attach_items_to( Object& ) const;
    void handle_object_collided( const Collide& ) const;
    Inventory* m_inventory;
};
}

