#pragma once

#include <thectci/id.hpp>
#include <yarrr/types.hpp>
#include <yarrr/object.hpp>
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

class ObjectAssigned : public Entity
{
  public:
    add_polymorphic_ctci( "object_assigned" );

    ObjectAssigned() = default;
    ObjectAssigned( const Object::Id& object_id );
    const Object::Id& object_id() const;
  private:
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    Object::Id m_object_id;
};

}

