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
    typedef std::function< Mission::Pointer() > Creator;

    MissionFactory();

    Mission::Pointer create_a( const std::string& name ) const;
    void register_creator( const Mission::Info& info, Creator&& );

  private:
    typedef std::unordered_map< std::string, Creator > Creators;
    Creators m_creators;

    the::model::OwningNode m_factory_model;
    the::model::Function m_register_model;
};

}

