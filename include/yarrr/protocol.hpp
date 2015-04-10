#pragma once

#include <string>

namespace yarrr
{

class Protocol
{
  public:
    const static std::string object_assigned;
    const static std::string login_request;
    const static std::string registration_request;
    const static std::string authentication_request;
    const static std::string authentication_response;
    const static std::string error;
};

}

