#include <yarrr/login.hpp>
#include <yarrr/object.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/id.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_login_request)
{
  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::LoginRequest::ctci ), Equals( true ) );
  }

  It( contains_a_login_id )
  {
    yarrr::LoginRequest login_request;
    AssertThat( login_request.login_id(), Equals("") );
  }

  It( can_be_constructed_with_the_id )
  {
    yarrr::LoginRequest login_request( login_id );
    AssertThat( login_request.login_id(), Equals( login_id ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::LoginRequest login_request( login_id );
    yarrr::Data data( login_request.serialize() );

    yarrr::LoginRequest deserialized_request;
    deserialized_request.deserialize( data );

    AssertThat( deserialized_request.login_id(), Equals( login_request.login_id() ) );
  }

  const std::string login_id{ "some id" };
};

Describe(a_login_response)
{
  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::LoginResponse::ctci ), Equals( true ) );
  }

  It( has_a_default_constructor )
  {
    yarrr::LoginResponse login_response;
    (void)login_response;
  }

  It( can_be_constructed_with_the_id )
  {
    yarrr::LoginResponse login_response( object_id );
    AssertThat( login_response.object_id(), Equals( object_id ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::LoginResponse login_response( object_id );
    yarrr::Data data( login_response.serialize() );

    yarrr::LoginResponse deserialized_request;
    deserialized_request.deserialize( data );

    AssertThat( deserialized_request.object_id(), Equals( login_response.object_id() ) );
  }

  const yarrr::Object::Id object_id{ 123 };
};
