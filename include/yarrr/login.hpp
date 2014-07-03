#pragma once

#include <thectci/id.hpp>
#include <yarrr/types.hpp>
#include <yarrr/object.hpp>

#include <string>

namespace yarrr
{

class LoginRequest
{
  public:
    add_ctci( "login_request" );

    LoginRequest() = default;
    LoginRequest( const std::string& login_id );
    const std::string& login_id() const;

    yarrr::Data serialize() const;
    void deserialize( const yarrr::Data& data );

  private:
    std::string m_login_id;
};

class LoginResponse
{
  public:
    add_ctci( "login_response" );

    LoginResponse() = default;
    LoginResponse( const yarrr::Object::Id& object_id );
    const yarrr::Object::Id& object_id() const;

    yarrr::Data serialize() const;
    void deserialize( const yarrr::Data& data );

  private:
    yarrr::Object::Id m_object_id;
};

}

