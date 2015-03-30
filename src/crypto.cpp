#include <yarrr/crypto.hpp>
#include "sha256.hpp"

namespace yarrr
{

std::string
auth_hash( const std::string& text )
{
  return sha256( text );
}

}

