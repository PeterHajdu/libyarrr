#include <yarrr/mission_container.hpp>
#include <yarrr/mission.hpp>
#include <yarrr/lua_engine.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_mission_container )
{

  void decorate_mission( yarrr::Mission& mission )
  {
    mission.add_objective( yarrr::Mission::Objective( "updater name",
          [ this ]( yarrr::Mission& ) -> yarrr::TaskState
          {
            was_mission_updated = true;
            return mission_state;
          } ) );
  }

  void add_mission()
  {
    yarrr::Mission::Pointer new_mission( new yarrr::Mission() );
    decorate_mission( *new_mission );
    mission_id = new_mission->id();
    mission_copy = *new_mission;
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

  bool container_contains( const yarrr::Mission::Id& id )
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
    AssertThat( container_contains( mission_id ), Equals( true ) );
  }

  It( updates_all_missions )
  {
    container->update();
    AssertThat( was_mission_updated, Equals( true ) );
  }

  It( deletes_missions_only_when_they_finish )
  {
    container->update();
    AssertThat( container_contains( mission_id ), Equals( true ) );
  }

  void finish_all()
  {
    mission_state = yarrr::succeeded;
    container->update();
  }

  It( deletes_finished_missions )
  {
    finish_all();
    AssertThat( container->missions(), HasLength( 0u ) );
  }

  It( calls_finished_callback_when_a_mission_gets_finished )
  {
    finish_all();
    AssertThat( was_mission_finished_callback_called, Equals( true ) );
  }

  void finish_mission( yarrr::Mission& mission )
  {
    decorate_mission( mission );
    mission_state = yarrr::succeeded;
    mission.update();
  }

  It( updates_the_mission_when_adding_one_with_an_existing_id )
  {
    finish_mission( mission_copy );
    container->add_mission( yarrr::Mission::Pointer( new yarrr::Mission( mission_copy ) ) );
    AssertThat( container_contains( mission_id ), Equals( false ) );
  }

  It( removes_immediately_new_but_finished_missions )
  {
    yarrr::Mission::Pointer new_mission( new yarrr::Mission() );
    const yarrr::Mission::Id new_mission_id{ new_mission->id() };
    finish_mission( *new_mission );
    container->add_mission( std::move( new_mission ) );
    AssertThat( container_contains( new_mission_id ), Equals( false ) );
  }

  std::unique_ptr< yarrr::MissionContainer > container;
  bool was_mission_updated;
  yarrr::TaskState mission_state;
  yarrr::Mission mission_copy;
  yarrr::Mission::Id mission_id;
  yarrr::Mission::Id mission_finished_id;
  bool was_mission_finished_callback_called;
};

