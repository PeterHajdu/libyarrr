#include <yarrr/lua_agent.hpp>
#include <yarrr/object.hpp>
#include <yarrr/timer_update.hpp>
#include <yarrr/object_decorator.hpp>

namespace yarrr
{

LuaAgent::LuaAgent( LuaFunction function, const the::time::Time& period )
  : ObjectBehavior( do_not_synchronize() )
  , m_function( function )
  , m_period( period )
  , m_last_called( 0u )
{
}


ObjectBehavior::Pointer
LuaAgent::clone() const
{
  return std::make_unique< LuaAgent >( m_function, m_period );
}

void
LuaAgent::do_register_to( Object& object )
{
  object.dispatcher.register_listener< TimerUpdate >(
      [ this ]( const TimerUpdate& update )
      {
        handle_timer_update( update );
      } );
}

void
LuaAgent::handle_timer_update( const TimerUpdate& update )
{
  if ( m_last_called > update.timestamp - m_period )
  {
    return;
  }

  m_last_called = update.timestamp;
  m_function.call( ObjectDecorator( *m_object ) );
}

}

