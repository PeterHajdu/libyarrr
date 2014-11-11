#include <yarrr/mission_container.hpp>
#include <yarrr/mission.hpp>
#include <yarrr/lua_engine.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe_Only( a_mission_container )
{

  void add_mission()
  {
    yarrr::Mission::Pointer new_mission( new yarrr::Mission() );
    new_mission->add_objective( yarrr::Mission::Objective( "updater name",
          [ this ]( const std::string& ) -> yarrr::TaskState
          {
            was_mission_updated = true;
            return mission_state;
          } ) );
    mission_id = new_mission->id();
    container->add_mission( std::move( new_mission ) );
  }

  void SetUp()
  {
    mission_state = yarrr::ongoing;
    mission_finished_id = 0;
    was_mission_finished_callback_called = false;
    container.reset( new yarrr::MissionContainer(
          [ this ]( const yarrr::Mission& finished_mission )
          {
            was_mission_finished_callback_called = true;
            mission_finished_id = finished_mission.id();
          } ) );
    was_mission_updated = false;
    add_mission();
  }

  bool contains( const yarrr::Mission::Id& id )
  {
    for ( const auto& mission : container->missions() )
    {
      if ( mission->id() == id )
      {
        return true;
      }
    }
    return false;
  }

  It( shows_stored_missions )
  {
    AssertThat( contains( mission_id ), Equals( true ) );
  }

  It( updates_all_missions )
  {
    container->update();
    AssertThat( was_mission_updated, Equals( true ) );
  }

  It( deletes_missions_only_when_they_finish )
  {
    container->update();
    AssertThat( contains( mission_id ), Equals( true ) );
  }

  void finish()
  {
    mission_state = yarrr::succeeded;
    container->update();
  }

  It( deletes_finished_missions )
  {
    finish();
    AssertThat( container->missions(), HasLength( 0u ) );
  }

  It( calls_finished_callback_when_a_mission_gets_finished )
  {
    finish();
    AssertThat( was_mission_finished_callback_called, Equals( true ) );
  }

  std::unique_ptr< yarrr::MissionContainer > container;
  bool was_mission_updated;
  yarrr::TaskState mission_state;
  yarrr::Mission::Id mission_id;
  yarrr::Mission::Id mission_finished_id;
  bool was_mission_finished_callback_called;
};

