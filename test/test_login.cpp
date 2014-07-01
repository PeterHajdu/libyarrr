#include <yarrr/login.hpp>
#include <thectci/id.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  template < typename T >
  class has_ctci
  {
    static const the::ctci::Id id{ T::ctci };
  };
}

Describe(a_login_request)
{
  It( has_a_default_constructor )
  {
    yarrr::LoginRequest login_request;
    (void)login_request;
  }

  It( has_a_ctci )
  {
    has_ctci< yarrr::LoginRequest > check;
    (void)check;
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

