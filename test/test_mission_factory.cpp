#include <yarrr/mission_factory.hpp>
#include <yarrr/mission.hpp>
#include <yarrr/lua_engine.hpp>
#include <thectci/service_registry.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_mission_factory )
{

  void SetUp()
  {
    factory = &the::ctci::service< yarrr::MissionFactory >();
    factory->register_creator( info,
        [ this ]()
        {
          return yarrr::Mission::Pointer( new yarrr::Mission( info ) );
        } );
  }

  It( returns_null_mission_objects_if_name_is_unknown )
  {
    yarrr::Mission::Pointer mission( factory->create_a( non_existing_mission_name ) );
    AssertThat( mission.get() == nullptr, Equals( true ) );
  }

  It( creates_one_of_the_named_mission_object )
  {
    yarrr::Mission::Pointer mission( factory->create_a( existing_mission_name ) );
    AssertThat( mission->name(), Equals( existing_mission_name ) );
    AssertThat( mission->description(), Equals( existing_mission_description ) );
  }

  It( exports_factory_registration_to_the_lua_world )
  {
    try
    {
      sol::state& lua( yarrr::Lua::state() );
      lua.new_userdata< yarrr::Mission::Info, std::string, std::string >( "MissionInfo" );

      lua.script(
          "function a_mission_factory( new_mission )\n"
          "a_mission_factory_was_called = true\n"
          "end\n"
          "register_mission_factory( MissionInfo.new( \"a\", \"b\" ), a_mission_factory )\n" );

      yarrr::Mission::Pointer mission( factory->create_a( "a" ) );
      lua.script( "assert( a_mission_factory_was_called )\n" );
      AssertThat( mission->name(), Equals( "a" ) );
      AssertThat( mission->description(), Equals( "b" ) );
    }
    catch ( std::exception& e )
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  yarrr::MissionFactory* factory;
  const std::string non_existing_mission_name{ "a non existing mission name" };
  const std::string existing_mission_name{ "existing mission name" };
  const std::string existing_mission_description{ "existing mission description" };
  const yarrr::Mission::Info info{ existing_mission_name, existing_mission_description };
};

