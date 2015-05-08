#pragma once

#include <yarrr/object.hpp>
#include <yarrr/item.hpp>

namespace yarrr
{

class Goods;
class CargoSpace;
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
    std::vector< Goods > collect_goods() const;
    virtual void do_register_to( Object& ) override;
    void handle_object_destroyed( const ObjectDestroyed& ) const;
    PhysicalParameters* m_owner_parameters;
    Inventory* m_inventory;
    CargoSpace* m_cargo;
    Shape* m_shape;
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
    void add_goods_to( Object& ) const;
    void handle_object_collided( const Collide& ) const;
    CargoSpace* m_cargo;
};

}

