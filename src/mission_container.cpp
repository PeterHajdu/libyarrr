#include <yarrr/mission_container.hpp>

namespace yarrr
{

MissionContainer::MissionContainer( FinishedCallback finished_callback )
  : m_finished_callback( finished_callback )
{
}

void
MissionContainer::add_mission( Mission::Pointer&& mission )
{
  m_missions.emplace_back( std::move( mission ) );
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

