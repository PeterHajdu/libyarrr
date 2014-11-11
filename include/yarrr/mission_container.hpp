#pragma once

#include <yarrr/mission.hpp>

namespace yarrr
{

class MissionContainer final
{
  public:
    typedef std::function< void( const Mission& ) > FinishedCallback;
    MissionContainer( FinishedCallback );
    void add_mission( Mission::Pointer&& mission );

    typedef std::vector< Mission::Pointer > Missions;
    const Missions& missions() const;
    void update();

  private:
    Missions m_missions;
    FinishedCallback m_finished_callback;
};

}

