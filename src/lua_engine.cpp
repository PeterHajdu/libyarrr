#include <yarrr/lua_engine.hpp>
#include <yarrr/log.hpp>

#include <memory>

namespace
{

yarrr::LuaEngine* engine_instance( nullptr );

}

namespace yarrr
{

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

bool
LuaEngine::run( const std::string& script )
{
  the::model::Lua& model{ instance().model() };
  const bool did_succeed{ model.run( script ) };
  if ( did_succeed )
  {
    thelog( log::debug )( "Lua script execution succeeded:\n", script );
    return did_succeed;
  }

  thelog( log::error )( "Lua script failed:\n", script, "Error message:\n", model.error_message() );
  return did_succeed;
}

AutoLuaRegister::AutoLuaRegister( std::function< void( LuaEngine& ) > register_function )
{
  LuaEngine& instance( LuaEngine::instance() );
  thelog( yarrr::log::debug )( "Auto registering stuff on lua engine::", &instance, &instance.state() );
  register_function( instance );
}

LuaFunction::LuaFunction( sol::function sol_function )
  : m_sol_function( sol_function )
{
}

}

