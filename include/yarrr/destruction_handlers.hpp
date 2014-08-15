#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class ObjectDestroyed;
class PhysicalParameters;

class DeleteWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_delete_when_destroyed" );
    DeleteWhenDestroyed();
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;

  private:
    void handle_object_destroyed( const ObjectDestroyed& ) const;
    Object::Id m_object_id;
};

class RespawnWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_respawn_when_destroyed" );

    RespawnWhenDestroyed();
    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;

  private:
    void handle_object_destroyed( const ObjectDestroyed& ) const;
    PhysicalParameters* m_physical_parameters;
};

}

