#include <yarrr/mission.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/log.hpp>
#include <yarrr/lua_engine.hpp>

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

Mission::Mission( const Info& info )
  : m_info( info )
  , m_state( ongoing )
  , m_context( Lua::state().create_table() )
{
  thelog( log::debug )( "Mission constructed.", m_info.name, m_info.description );
}


Mission::Mission( const Mission& other )
  : Mission( other.m_info )
{
  thelog( log::debug )( "Mission copy constructed.", m_info.name, m_info.description );
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
  return m_info.name;
}

const std::string&
Mission::description() const
{
  return m_info.description;
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
  serializer.push_back( m_info.name );
  serializer.push_back( m_info.description );
  serializer.push_back( m_state );
  serializer.push_back( uint32_t( m_objectives.size() ) );
  for ( const auto& objective : m_objectives )
  {
    objective.serialize( serializer );
  }
}

void
Mission::do_deserialize( Deserializer& deserializer )
{
  m_info.name = deserializer.pop_front< std::string >();
  m_info.description = deserializer.pop_front< std::string >();
  m_state = deserializer.pop_front< TaskState >();
  const uint32_t number_of_objectives( deserializer.pop_front< uint32_t >() );
  for ( uint32_t i( 0 ); i < number_of_objectives; ++i )
  {
    m_objectives.emplace_back();
    m_objectives.back().deserialize( deserializer );
  }
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

Mission::Pointer
Mission::create( const Info& info )
{
  return Pointer( new Mission( info ) );
}

}

