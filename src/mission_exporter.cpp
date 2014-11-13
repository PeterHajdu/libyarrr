#include <yarrr/mission_exporter.hpp>

namespace yarrr
{

MissionModel::MissionModel( const std::string& mission_id, const std::string& object_id, the::model::OwningNode& parent )
  : the::model::OwningNode( mission_id, parent )
  , m_character( object_id, *this )
{
}

}


