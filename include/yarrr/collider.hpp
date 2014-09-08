#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class PhysicalBehavior;

class Collide
{
  public:
    add_ctci( "yarrr_collide" );
    Collide( Object& );
    yarrr::Object& with;
};

class ObjectDestroyed
{
  public:
    add_ctci( "yarrr_object_destroyed" );
};

class Collider : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_collider" );

    enum : int
    {
      ship_layer,
      laser_layer,
      loot_layer
    };

    Collider( int layer );

    virtual Pointer clone() const override;
    virtual ~Collider() = default;

    void collide_if_needed_with( Collider& );

  private:
    virtual void do_register_to( Object& ) override;
    bool is_collider_too_far( const Collider& ) const;
    void collide_with( const Collider& ) const;

    yarrr::PhysicalBehavior* m_physical_behavior;
    const int m_layer;
};

class DamageCauser : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_damage_causer" );
    DamageCauser( int initial_integrity );
    virtual ~DamageCauser() = default;

    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;
    void handle_collision( const Collide& ) const;
    void reset_integrity() const;

    const int m_initial_integrity;
    int16_t* m_integrity;
};

}

