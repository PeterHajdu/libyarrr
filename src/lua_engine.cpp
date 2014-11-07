#include <yarrr/lua_engine.hpp>
#include <yarrr/log.hpp>

#include <memory>

namespace
{

yarrr::LuaEngine* engine_instance( nullptr );

}

namespace yarrr
{

bool
lua_script( the::model::Lua& lua, const std::string& script )
{
  thelog( log::debug )( "Executing lua script:", script );
  if ( !lua.run( script ) )
  {
    thelog( log::debug )( "Lua script failed with error message:", lua.error_message() );
    return false;
  }

  return true;
}

LuaEngine::LuaEngine()
{
  m_lua.state().open_libraries( sol::lib::base );
  m_lua.state().open_libraries( sol::lib::package );
  m_lua.state().open_libraries( sol::lib::math );
  m_lua.state().open_libraries( sol::lib::os );
}

LuaEngine& LuaEngine::instance()
{
  if ( !engine_instance )
  {
    thelog( log::debug )( "Creating lua engine instance.", engine_instance );
    engine_instance = new LuaEngine();
  }

  return *engine_instance;
}

AutoLuaRegister::AutoLuaRegister( std::function< void( LuaEngine& ) > register_function )
{
  LuaEngine& instance( LuaEngine::instance() );
  thelog( yarrr::log::debug )( "Auto registering stuff on lua engine::", &instance, &instance.state() );
  register_function( instance );
}

}

