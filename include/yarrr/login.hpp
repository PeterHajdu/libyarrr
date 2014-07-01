#pragma once

#include <thectci/id.hpp>
#include <yarrr/types.hpp>

#include <string>

namespace yarrr
{

class LoginRequest
{
  public:
    add_ctci( "login" );

    LoginRequest() = default;
    LoginRequest( const std::string& login_id );
    const std::string& login_id() const;

    yarrr::Data serialize() const;
    void deserialize( const yarrr::Data& data );

  private:
    std::string m_login_id;
};

}

