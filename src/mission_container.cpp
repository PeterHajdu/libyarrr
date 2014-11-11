#include <yarrr/mission_container.hpp>

namespace yarrr
{

MissionContainer::MissionContainer( FinishedCallback finished_callback )
  : m_finished_callback( finished_callback )
{
}

void
MissionContainer::add_mission( Mission::Pointer&& new_mission )
{
  const auto mission_iterator( std::find_if( std::begin( m_missions ), std::end( m_missions ),
       [ &new_mission ]( const Mission::Pointer& mission )
       {
         return new_mission->id() == mission->id();
       } ) );

  if ( mission_iterator != std::end( m_missions ) )
  {
    mission_iterator->swap( new_mission );
    return;
  }

  m_missions.emplace_back( std::move( new_mission ) );
}

const MissionContainer::Missions&
MissionContainer::missions() const
{
  return m_missions;
}

void
MissionContainer::update()
{
  auto mission( std::begin( m_missions ) );
  while ( mission != std::end( m_missions ) )
  {
    (*mission)->update();
    const bool is_finished( ongoing != (*mission)->state() );
    if ( is_finished )
    {
      m_finished_callback( **mission );
      mission = m_missions.erase( mission );
      continue;
    }

    ++mission;
  }
}

}

