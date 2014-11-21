#pragma once

#include <yarrr/mission.hpp>
#include <unordered_map>
#include <themodel/function.hpp>
#include <themodel/node.hpp>

namespace yarrr
{

class MissionFactory final
{
  public:
    add_ctci( "yarrr_mission_factory" );
    using Creator = std::function< Mission::Pointer() >;

    MissionFactory();

    Mission::Pointer create_a( const std::string& name ) const;
    void register_creator( const Mission::Info& info, Creator&& );

    using MissionList = std::vector< std::string >;
    const MissionList& missions() const;

  private:
    typedef std::unordered_map< std::string, Creator > Creators;
    Creators m_creators;

    the::model::OwningNode m_factory_model;
    the::model::Function m_register_model;

    MissionList m_mission_list;
};

}

