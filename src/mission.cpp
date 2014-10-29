#include <yarrr/mission.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/log.hpp>

#include <algorithm>

namespace
{

yarrr::AutoEntityRegister< yarrr::Mission > auto_mission_register;

yarrr::Mission::Objective::Updater
wrap_lua_updater( sol::function updater )
{
  return
    [ updater ]( sol::table& context ) -> yarrr::TaskState
    {
      thelog( yarrr::log::debug )( "Calling lua updater." );
      return yarrr::TaskState( updater.call<int>( context ) );
    };
}

}

namespace yarrr
{

Mission::Mission( const std::string& name, const std::string& description )
  : m_name( name )
  , m_description( description )
  , m_state( ongoing )
  , m_context( Lua::state().create_table() )
{
  thelog( log::debug )( "Mission constructed.", m_name, m_description );
}


Mission::Mission( const Mission& other )
  : Mission( other.m_name, other.m_description )
{
  thelog( log::debug )( "Mission copy constructed.", m_name, m_description );
}

void
Mission::update()
{
  for ( auto& objective : m_objectives )
  {
    objective.update( m_context );
  }

  calculate_mission_state();
}

void
Mission::calculate_mission_state()
{
  if ( std::all_of( std::begin( m_objectives ), std::end( m_objectives ),
        []( const Objective& objective ){ return objective.state() == succeeded; } ) )
  {
    m_state = succeeded;
    return;
  }

  if ( std::any_of( std::begin( m_objectives ), std::end( m_objectives ),
        []( const Objective& objective ){ return objective.state() == failed; } ) )
  {
    m_state = failed;
    return;
  }
}

const TaskState
Mission::state() const
{
  return m_state;
}

const std::string&
Mission::name() const
{
  return m_name;
}

const std::string&
Mission::description() const
{
  return m_description;
}

void
Mission::add_objective( const Objective& objective )
{
  m_objectives.emplace_back( objective );
}

const Mission::Objectives&
Mission::objectives() const
{
  return m_objectives;
}

void
Mission::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_name );
  serializer.push_back( m_description );
}

void
Mission::do_deserialize( Deserializer& deserializer )
{
  m_name = deserializer.pop_front< std::string >();
  m_description = deserializer.pop_front< std::string >();
}


Mission::Objective::Objective( const std::string& description, Updater updater )
  : m_description( description )
  , m_updater( updater )
  , m_state( ongoing )
{
  thelog( log::debug )( "Mission objective created.", description );
}

Mission::Objective::Objective( const Objective& other )
  : Objective( other.m_description, other.m_updater )
{
  thelog( log::debug )( "Mission objective copy constructed.", m_description );
}

Mission::Objective::Objective( const std::string& description, sol::function updater )
  : Objective( description, wrap_lua_updater( updater ) )
{
  thelog( log::debug )( "Mission objective created with lua updater.", description );
}

const std::string&
Mission::Objective::description() const
{
  return m_description;
}

const TaskState
Mission::Objective::state() const
{
  return m_state;
}

void
Mission::Objective::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_description );
  serializer.push_back( m_state );
}

void
Mission::Objective::do_deserialize( Deserializer& deserializer )
{
  m_description = deserializer.pop_front< std::string >();
  m_state = deserializer.pop_front< TaskState >();
}


void
Mission::Objective::update( sol::table& context )
{
  m_state = m_updater( context );
}

}

