#include <yarrr/mission.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/lua_engine.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_mission )
{

  void SetUp()
  {
    mission.reset( new yarrr::Mission( yarrr::Mission::Info{ name, description } ) );
    objective_return_values.clear();
    objective_updated_times.clear();
  }

  It( has_a_unique_id )
  {
    yarrr::Mission another_mission( yarrr::Mission::Info{ name, description } );
    AssertThat( mission->id(), !Equals( another_mission.id() ) );
  }

  It( has_default_constructor )
  {
    yarrr::Mission a_mission;
    (void)a_mission;
  }

  It( has_a_name_and_description )
  {
    AssertThat( mission->name(), Equals( name ) );
    AssertThat( mission->description(), Equals( description ) );
  }

  It( is_serializable )
  {
    add_objectives( 4 );
    fail_mission();
    const yarrr::Data serialized_mission( mission->serialize() );
    yarrr::Mission deserialized_mission;
    deserialized_mission.deserialize( serialized_mission );
    AssertThat( deserialized_mission.name(), Equals( name ) );
    AssertThat( deserialized_mission.description(), Equals( description ) );
    AssertThat( deserialized_mission.state(), Equals( yarrr::failed ) );
    AssertThat( deserialized_mission.objectives(), HasLength( 4 ) );
    AssertThat( deserialized_mission.id(), Equals( mission->id() ) );
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::Mission::ctci ), Equals( true ) );
  }

  It( has_a_default_state_ongoing )
  {
    AssertThat( mission->state(), Equals( yarrr::ongoing ) );
  }


  void add_objectives( size_t count )
  {
    for ( size_t i( 0 ); i < count; ++i )
    {
      objective_return_values.push_back( yarrr::ongoing );
      objective_updated_times.push_back( 0 );

      yarrr::Mission::Objective an_objective(
          objective_description,
          [ this, i ]( yarrr::Mission& m )
          {
            AssertThat( &m, Equals( mission.get() ) );
            ++objective_updated_times[ i ];
            return objective_return_values.at( i );
          } );

      mission->add_objective( an_objective );
    }
    AssertThat( mission->objectives(), HasLength( count ) );
  }

  It( has_objectives )
  {
    add_objectives( 1u );
    AssertThat( mission->objectives().back().description(), Equals( objective_description ) );
  }

  It( copies_objectives_when_copy_constructed )
  {
    add_objectives( 1u );
    yarrr::Mission copy( *mission );
    AssertThat( mission->objectives().back().description(), Equals( copy.objectives().back().description() ) );
  }

  It( updates_all_objectives_during_update )
  {
    add_objectives( 5u );
    mission->update();
    for ( const auto& update_time : objective_updated_times )
    {
      AssertThat( update_time, Equals( 1 ) );
    }
  }

  void fail_mission()
  {
    mission->update();
    AssertThat( mission->state(), Equals( yarrr::ongoing ) );
    objective_return_values.back() = yarrr::failed;
    mission->update();
  }

  It( fails_if_any_of_the_objectives_fail )
  {
    add_objectives( 5u );
    fail_mission();
    AssertThat( mission->state(), Equals( yarrr::failed ) );
  }

  It( succeeds_only_if_all_of_the_objectives_succeed )
  {
    add_objectives( 5u );
    objective_return_values.back() = yarrr::succeeded;
    mission->update();
    AssertThat( mission->state(), Equals( yarrr::ongoing ) );
    std::fill( std::begin( objective_return_values ), std::end( objective_return_values ), yarrr::succeeded );
    mission->update();
    AssertThat( mission->state(), Equals( yarrr::succeeded ) );
  }

  yarrr::Mission::Pointer mission;
  const std::string name{  "this is the name" };
  const std::string description{ "and this is the description" };

  std::vector< yarrr::TaskState > objective_return_values;
  std::vector< int > objective_updated_times;
  const std::string objective_description{ "an objective" };
};

Describe( a_mission_objective )
{
  void set_up_lua_stuff()
  {
    lua.reset( new the::model::Lua() );
    lua->state()[ "objective_succeeded" ] = int( yarrr::succeeded );

    lua->state().new_userdata< yarrr::Mission, yarrr::Mission::Info >( "Mission",
        "add_objective", &yarrr::Mission::add_objective,
        "id", &yarrr::Mission::string_id );

    lua->state().new_userdata< yarrr::Mission::Objective, std::string, sol::function >( "MissionObjective" );
    lua->run( "function updater( mission )\nprint( \"updater was called:\"..mission:id() )\nupdated_mission_id = mission:id()\nreturn objective_succeeded\nend\n" );
    lua->run( "function create_objective()\nreturn MissionObjective.new( \"bla\", updater )\nend\n" );

    lua->run( "function crashing_updater( id )\nassert( false )\nend\n" );
    lua->run( "function create_crashing_objective()\nreturn MissionObjective.new( \"blasldkfj\", crashing_updater )\nend\n" );
  }

  void SetUp()
  {
    was_updater_called = false;
    updater_should_return = yarrr::ongoing;
    called_with_mission_id.clear();
    objective.reset( new yarrr::Mission::Objective(
          description,
          [ this ]( yarrr::Mission& m ) -> yarrr::TaskState
          {
            called_with_mission_id = m.id();
            was_updater_called = true;
            return updater_should_return;
          } ) );
    set_up_lua_stuff();
  }

  It( has_a_description )
  {
    AssertThat( objective->description(), Equals( description ) );
  }

  It( is_ongoing_by_default )
  {
    AssertThat( objective->state(), Equals( yarrr::ongoing ) );
  }

  It( can_be_serialized )
  {
    const yarrr::Data serialized_objective( objective->serialize() );
    yarrr::Mission::Objective deserialized_objective;
    deserialized_objective.deserialize( serialized_objective );

    AssertThat( deserialized_objective.description(), Equals( description ) );
  }

  void finish_with_success()
  {
    updater_should_return = yarrr::succeeded;
    objective->update( mission );
  }

  It( serializes_objective_state )
  {
    finish_with_success();
    const yarrr::Data serialized_objective( objective->serialize() );
    yarrr::Mission::Objective deserialized_objective;
    deserialized_objective.deserialize( serialized_objective );

    AssertThat( deserialized_objective.description(), Equals( description ) );
    AssertThat( deserialized_objective.state(), Equals( yarrr::succeeded ) );
  }

  It( calls_state_updater_when_updating )
  {
    objective->update( mission );
    AssertThat( was_updater_called, Equals( true ) );
  }

  void assert_update_to_should_be( yarrr::TaskState returns, yarrr::TaskState expected )
  {
    updater_should_return = returns;
    objective->update( mission );
    AssertThat( objective->state(), Equals( expected ) );
  }

  It( updates_its_state_according_to_the_state_updater )
  {
    assert_update_to_should_be( yarrr::succeeded, yarrr::succeeded );
  }

  It( copies_its_state_when_copy_constructed )
  {
    assert_update_to_should_be( yarrr::succeeded, yarrr::succeeded );
    yarrr::Mission::Objective a_copy( *objective );
    AssertThat( a_copy.state(), Equals( yarrr::succeeded ) );
  }

  It( should_stay_succeeded )
  {
    assert_update_to_should_be( yarrr::succeeded, yarrr::succeeded );
    assert_update_to_should_be( yarrr::ongoing, yarrr::succeeded );
    assert_update_to_should_be( yarrr::failed, yarrr::succeeded );
  }

  It( should_stay_failed )
  {
    assert_update_to_should_be( yarrr::failed, yarrr::failed );
    assert_update_to_should_be( yarrr::ongoing, yarrr::failed );
    assert_update_to_should_be( yarrr::succeeded, yarrr::failed );
  }

  It( passes_mission_id_to_the_lua_updater )
  {
    yarrr::Mission::Objective objective( lua->state()[ "create_objective" ].call<yarrr::Mission::Objective&>() );
    objective.update( mission );
    AssertThat( lua->assert_equals( "updated_mission_id", mission.string_id() ), Equals( true ) );
  }

  It( updates_objective_state_according_to_lua_updater )
  {
    yarrr::Mission::Objective objective( lua->state()[ "create_objective" ].call<yarrr::Mission::Objective&>() );
    objective.update( mission );
    AssertThat( objective.state(), Equals( yarrr::succeeded ) );
  }

  It( should_catch_lua_exceptions )
  {
    yarrr::Mission::Objective objective( lua->state()[ "create_crashing_objective" ].call<yarrr::Mission::Objective&>() );
    objective.update( mission );
  }

  std::unique_ptr< the::model::Lua > lua;
  yarrr::Mission::Objective::Pointer objective;
  const std::string description{ "objective description" };
  bool was_updater_called;
  yarrr::Mission mission;
  std::string called_with_mission_id;
  yarrr::TaskState updater_should_return;
};

