#pragma once

#include <thectci/id.hpp>
#include <yarrr/types.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/entity.hpp>

#include <string>

namespace yarrr
{

class LoginRequest : public Entity
{
  public:
    add_polymorphic_ctci( "login_request" );

    LoginRequest() = default;
    LoginRequest( const std::string& login_id );
    const std::string& login_id() const;
  private:
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    std::string m_login_id;
};

class LoginResponse : public Entity
{
  public:
    add_polymorphic_ctci( "login_response" );

    LoginResponse() = default;
    LoginResponse( const yarrr::PhysicalParameters::Id& object_id );
    const yarrr::PhysicalParameters::Id& object_id() const;
  private:
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    yarrr::PhysicalParameters::Id m_object_id;
};

}

