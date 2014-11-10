#include <yarrr/mission_exporter.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_mission_model )
{
  void SetUp()
  {
    lua.reset( new the::model::Lua() );
    missions.reset( new yarrr::MissionsModel( missions_name, *lua ) );
    missions->add_node( the::model::Node::Pointer(
          new yarrr::MissionModel( mission_id, object_id, *missions ) ) );
  }

  void TearDown()
  {
    mission.reset();
    missions.reset();
    lua.reset();
  }

  It( exports_the_characters_object_id )
  {
    AssertThat(
        lua->assert_equals(
          the::model::path_from( {
            the::model::index_lua_table( missions_name, mission_id ),
            "character",
            "object_id" } ),
          object_id ),
        Equals( true ) );
  }

  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< yarrr::MissionsModel > missions;
  std::unique_ptr< yarrr::MissionModel > mission;

  const std::string missions_name{ "missions" };
  const std::string mission_id{ "209329834" };
  const std::string object_id{ "293847" };
};

