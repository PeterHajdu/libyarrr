#include <yarrr/lua_engine.hpp>
#include <yarrr/log.hpp>

#include <memory>

namespace
{

yarrr::Lua* engine_instance( nullptr );

}

namespace yarrr
{

Lua::Lua()
{
  m_state.open_libraries( sol::lib::base );
  m_state.open_libraries( sol::lib::package );
  m_state.open_libraries( sol::lib::math );
  m_state.open_libraries( sol::lib::os );
}

Lua& Lua::instance()
{
  if ( !engine_instance )
  {
    thelog( log::debug )( "Creating lua engine instance.", engine_instance );
    engine_instance = new Lua();
  }

  return *engine_instance;
}

AutoLuaRegister::AutoLuaRegister( std::function< void( Lua& ) > register_function )
{
  Lua& instance( Lua::instance() );
  thelog( yarrr::log::debug )( "Auto registering stuff on lua engine::", &instance, &instance.state() );
  register_function( instance );
}

}

