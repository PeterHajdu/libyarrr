#include <yarrr/mission.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/lua_engine.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_mission )
{

  void SetUp()
  {
    mission.reset( new yarrr::Mission( name, description ) );
    objective_return_values.clear();
    objective_updated_times.clear();
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
    const yarrr::Data serialized_mission( mission->serialize() );
    yarrr::Mission deserialized_mission;
    deserialized_mission.deserialize( serialized_mission );
    AssertThat( deserialized_mission.name(), Equals( name ) );
    AssertThat( deserialized_mission.description(), Equals( description ) );
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
          [ this, i ]( sol::table& )
          {
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

  Describe( lua_behavior )
  {
    void SetUp()
    {
      lua.reset( new sol::state() );
      lua->new_userdata< yarrr::Mission, std::string, std::string >( "Mission" );
      lua->script( "function new_mission()\nreturn Mission.new( \"mission in lua\", \"with description\" )\nend\n" );
      new_mission.reset( new sol::function( (*lua)[ "new_mission" ] ) );
    }

    It( can_be_constructed )
    {
      yarrr::Mission mission( new_mission->call< yarrr::Mission& >() );
      AssertThat( mission.name(), Equals( "mission in lua" ) );
      AssertThat( mission.description(), Equals( "with description" ) );
    }

    std::unique_ptr< sol::state > lua;
    std::unique_ptr< sol::function > new_mission;
  };

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
    lua.reset( new sol::state() );
    context = lua->create_table();
    (*lua)[ "objective_succeeded" ] = int( yarrr::succeeded );
    lua->new_userdata< yarrr::Mission::Objective, std::string, sol::function >( "MissionObjective" );
    lua->script( "function updater( context )\ncontext[ \"123\" ]=123\nreturn objective_succeeded\nend\n" );
    lua->script( "function create_objective()\nreturn MissionObjective.new( \"bla\", updater )\nend\n" );
  }

  void SetUp()
  {
    was_updater_called = false;
    updater_should_return = yarrr::ongoing;
    objective.reset( new yarrr::Mission::Objective(
          description,
          [ this ]( sol::table& ) -> yarrr::TaskState
          {
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
    objective->update( context );
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
    objective->update( context );
    AssertThat( was_updater_called, Equals( true ) );
  }

  It( updates_its_state_according_to_the_state_updater )
  {
    updater_should_return = yarrr::succeeded;
    objective->update( context );
    AssertThat( objective->state(), Equals( yarrr::succeeded ) );
  }

  It( passes_context_table_to_the_lua_updater )
  {
    yarrr::Mission::Objective objective( (*lua)[ "create_objective" ].call<yarrr::Mission::Objective&>() );
    objective.update( context );
    AssertThat( context.get< int >( "123" ), Equals( 123 ) );
  }

  It( updates_objective_state_according_to_lua_updater )
  {
    yarrr::Mission::Objective objective( (*lua)[ "create_objective" ].call<yarrr::Mission::Objective&>() );
    objective.update( context );
    AssertThat( objective.state(), Equals( yarrr::succeeded ) );
  }

  std::unique_ptr< sol::state > lua;
  sol::table context;
  yarrr::Mission::Objective::Pointer objective;
  const std::string description{ "objective description" };
  bool was_updater_called;
  yarrr::TaskState updater_should_return;
};

