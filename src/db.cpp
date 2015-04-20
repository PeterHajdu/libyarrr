#include <yarrr/db.hpp>

namespace yarrr
{

std::string
user_key_from_id( const std::string& user_id )
{
  return std::string( "user:" ) + user_id;
}

}

