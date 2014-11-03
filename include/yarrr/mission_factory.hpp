#pragma once

#include <yarrr/mission.hpp>
#include <unordered_map>

namespace yarrr
{

class MissionFactory final
{
  public:
    add_ctci( "yarrr_mission_factory" );
    typedef std::function< Mission::Pointer() > Creator;
    Mission::Pointer create_a( const std::string& name ) const;
    void register_creator( const Mission::Info& info, Creator&& );

  private:
    typedef std::unordered_map< std::string, Creator > Creators;
    Creators m_creators;

};

}

