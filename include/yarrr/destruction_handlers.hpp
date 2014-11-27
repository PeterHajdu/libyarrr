#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class ObjectDestroyed;
class PhysicalParameters;

class CallWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_call_when_destroyed" );

    using Callback = std::function< void( const Object& ) >;
    CallWhenDestroyed( Callback );
    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;
    void handle_object_destroyed( const ObjectDestroyed& ) const;

    Callback m_callback;
};

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


class PlayerKilled
{
  public:
    add_ctci( "yarrr_player_killed" );
    PlayerKilled( const Object::Id& id );
    const Object::Id object_id;
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
};

}

