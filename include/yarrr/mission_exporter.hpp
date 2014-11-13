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
    template < typename Parent >
    MissionModel( const std::string& mission_id, const std::string& object_id, Parent& parent )
      : the::model::OwningNode( mission_id, parent )
      , m_character( object_id, *this )
    {
    }

  private:
    Character m_character;
};

}

