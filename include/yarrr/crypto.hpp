#pragma once

#include <string>

namespace yarrr
{

std::string
auth_hash( const std::string& );

std::string
random( size_t len );

}

