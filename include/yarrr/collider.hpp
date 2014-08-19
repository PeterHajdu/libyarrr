#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class PhysicalBehavior;

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
      laser_layer
    };

    Collider( int layer, int16_t initial_integrity, int16_t caused_damage );

    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;
    virtual ~Collider() = default;

    void collide_if_needed_with( Collider& );

  private:
    int16_t& current_integrity() const;
    bool is_collider_too_far( const Collider& ) const;
    void collide_with( const Collider& ) const;
    void reset_integrity() const;

    yarrr::PhysicalBehavior* m_physical_behavior;
    const int16_t m_initial_integrity;
    const int16_t m_caused_damage;
    the::ctci::Dispatcher* m_dispatcher;
    const int m_layer;
};

}

