#pragma once

#include <yarrr/character.hpp>
#include <themodel/node.hpp>
#include <themodel/node_list.hpp>

namespace yarrr
{

typedef the::model::OwningNodeList MissionsModel;

class MissionModel : public the::model::OwningNode
{
  public:
    MissionModel( const std::string& mission_id, const std::string& object_id, the::model::OwningNode& parent );

  private:
    Character m_character;
};

}

