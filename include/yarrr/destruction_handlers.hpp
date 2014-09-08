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
    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;

    void handle_object_destroyed( const ObjectDestroyed& ) const;
};

class RespawnWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_respawn_when_destroyed" );

    RespawnWhenDestroyed();
    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;

    void handle_object_destroyed( const ObjectDestroyed& ) const;
    PhysicalParameters* m_physical_parameters;
};

}

