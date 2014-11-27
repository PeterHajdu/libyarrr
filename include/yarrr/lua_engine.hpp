#pragma once

#include <sol.hpp>
#include <yarrr/log.hpp>
#include <themodel/lua.hpp>

namespace yarrr
{

class LuaEngine final
{
  public:
    static LuaEngine& instance();
    static sol::state& state()
    {
      return instance().m_lua.state();
    }

    static the::model::Lua& model()
    {
      return instance().m_lua;
    }

    static bool run( const std::string& script );

  private:
    LuaEngine();

    the::model::Lua m_lua;
};

class LuaFunction
{
  public:
    LuaFunction( sol::function sol_function );

    template < typename...Args >
    bool call( Args&&...args ) const
    {
      thelog( log::debug )( "Calling LuaFunction." );
      try
      {
        m_sol_function( std::forward< Args >( args )... );
      }
      catch( std::exception& e )
      {
        thelog( log::debug )( "LuaFunction failed:", e.what() );
        return false;
      }
      thelog( log::debug )( "LuaFunction finished with success." );
      return true;
    }

  private:
    sol::function m_sol_function;
};

class AutoLuaRegister final
{
  public:
    AutoLuaRegister( std::function< void( LuaEngine& ) > );
};

template < typename ClassToBeRegistered, typename...Args >
class SimpleLuaRegister
{
  public:
    SimpleLuaRegister( const char* name )
    {
      LuaEngine::state().new_userdata< ClassToBeRegistered, Args... >( name );
      thelog( yarrr::log::info )( "Expose class:", name, "to lua world." );
    }
};

}

