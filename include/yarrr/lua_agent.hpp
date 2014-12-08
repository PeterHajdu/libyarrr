#pragma once

#include <yarrr/object_behavior.hpp>
#include <yarrr/lua_engine.hpp>
#include <thetime/clock.hpp>

namespace yarrr
{
class TimerUpdate;

class LuaAgent : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_lua_agent" );
    LuaAgent( LuaFunction, const the::time::Time& period );
    virtual Pointer clone() const override final;

  private:
    virtual void do_register_to( Object& ) override final;

    void handle_timer_update( const TimerUpdate& );
    LuaFunction m_function;
    the::time::Time m_period;
    the::time::Time m_last_called;
};

}

