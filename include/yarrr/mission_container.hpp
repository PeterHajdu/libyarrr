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
    void fail_missions();

    typedef std::vector< Mission::Pointer > Missions;
    const Missions& missions() const;
    void update();

  private:
    void delete_finished_missions();

    Missions m_missions;
    FinishedCallback m_finished_callback;
};

}

