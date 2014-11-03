#include <yarrr/mission_factory.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/log.hpp>
#include <thectci/service_registry.hpp>

namespace
{
  void register_mission_factory( const yarrr::Mission::Info& info, sol::function factory )
  {
    the::ctci::service< yarrr::MissionFactory >().register_creator(
        info,
        [ factory, info ]()
        {
          thelog( yarrr::log::info )( "Calling lua mission factory method:", info.name );
          try
          {
            auto new_mission( yarrr::Mission::create( info ) );
            thelog( yarrr::log::debug )( "Sending object to lua mission factory method:", new_mission.get() );
            factory.call( *new_mission );
            return std::move( new_mission );
          }
          catch( std::exception& e )
          {
            thelog( yarrr::log::info )( "Lua mission factory method failed:", info.name, e.what() );
          }

          return yarrr::Mission::Pointer( nullptr );
        } );
    thelog( yarrr::log::info )( "Registered lua mission factory method for mission type:", info.name );
  }

  yarrr::AutoLuaRegister function_register(
      []( yarrr::Lua& lua )
      {
        lua.state().set_function( "register_mission_factory", register_mission_factory );
        thelog( yarrr::log::debug )( "Registered register_mission_factory method on lua state:", &lua.state() );
      } );

}

namespace yarrr
{

Mission::Pointer
MissionFactory::create_a( const std::string& name ) const
{
  Creators::const_iterator creator( m_creators.find( name ) );
  if ( creator == m_creators.end() )
  {
    return nullptr;
  }

  return creator->second();
}

void
MissionFactory::register_creator( const Mission::Info& info, Creator&& creator )
{
  m_creators.emplace( std::make_pair( info.name, std::move( creator ) ) );
}

}

