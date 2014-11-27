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

