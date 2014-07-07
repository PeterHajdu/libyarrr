#include <yarrr/login.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/object.hpp>
#include <yarrr/event_factory.hpp>

namespace
{
  yarrr::AutoEventRegister<yarrr::LoginRequest> register_login_request;
  yarrr::AutoEventRegister<yarrr::LoginResponse> register_login_response;
}

namespace yarrr
{

LoginRequest::LoginRequest( const std::string& login_id )
  : m_login_id( login_id )
{
}

const std::string&
LoginRequest::login_id() const
{
  return m_login_id;
}

void
LoginRequest::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_login_id );
}

void
LoginRequest::do_deserialize( Deserializer& deserializer )
{
  m_login_id = deserializer.pop_front<std::string>();
}

LoginResponse::LoginResponse( const yarrr::Object::Id& object_id )
  : m_object_id( object_id )
{
}

const yarrr::Object::Id&
LoginResponse::object_id() const
{
  return m_object_id;
}

void
LoginResponse::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_object_id );
}

void
LoginResponse::do_deserialize( Deserializer& deserializer )
{
  m_object_id = deserializer.pop_front<Object::Id>();
}

}

