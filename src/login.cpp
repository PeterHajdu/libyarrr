#include <yarrr/login.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/object.hpp>
#include <cassert>

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

yarrr::Data
LoginRequest::serialize() const
{
  yarrr::Data buffer;
  yarrr::Serializer serializer( buffer );
  serializer.push_back( ctci );
  serializer.push_back( m_login_id );
  return buffer;
}

void
LoginRequest::deserialize( const yarrr::Data& data )
{
  yarrr::Deserializer deserializer( data );
  assert( deserializer.pop_front< the::ctci::Id >() == ctci );
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

yarrr::Data
LoginResponse::serialize() const
{
  yarrr::Data buffer;
  yarrr::Serializer( buffer )
    .push_back( ctci )
    .push_back( m_object_id );
  return buffer;
}

void
LoginResponse::deserialize( const yarrr::Data& data )
{
  yarrr::Deserializer deserializer( data );
  assert( deserializer.pop_front< the::ctci::Id >() == ctci );
  m_object_id = deserializer.pop_front<yarrr::Object::Id>();
}
}

