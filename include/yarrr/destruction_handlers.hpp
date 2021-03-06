#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class ObjectDestroyed;
class PhysicalParameters;
class LuaFunction;

class CallWhenDestroyed : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_call_when_destroyed" );

    using Callback = std::function< void( Object& ) >;
    CallWhenDestroyed( Callback );
    CallWhenDestroyed( const LuaFunction& );
    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;
    void handle_object_destroyed( const ObjectDestroyed& ) const;

    Callback m_callback;
};

class PlayerKilled
{
  public:
    add_ctci( "yarrr_player_killed" );
    PlayerKilled( const Object::Id& id );
    const Object::Id object_id;
};

ObjectBehavior::Pointer delete_when_destroyed();
ObjectBehavior::Pointer kill_player_when_destroyed();

}

