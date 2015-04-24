#include <yarrr/db.hpp>

namespace yarrr
{

std::string
player_key_from_id( const std::string& player_id )
{
  return std::string( "player:" ) + player_id;
}

}

