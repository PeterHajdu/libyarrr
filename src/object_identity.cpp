#include <yarrr/object_identity.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>

namespace
{
  yarrr::AutoEntityRegister< yarrr::ObjectIdentity > auto_object_identity_register;
}

namespace yarrr
{

ObjectIdentity::ObjectIdentity()
  : ObjectBehavior( yarrr::synchronize_nth( 50 ) )
{
}

ObjectIdentity::ObjectIdentity( std::string captain )
  : ObjectBehavior( yarrr::synchronize_nth( 50 ) )
  , m_captain( std::move( captain ) )
{
}


ObjectIdentity::ObjectIdentity( const ObjectIdentity& other )
  : ObjectBehavior( yarrr::synchronize_nth( 50 ), other.id() )
  , m_captain( other.m_captain )
{
}


ObjectBehavior::Pointer
ObjectIdentity::clone() const
{
  return std::make_unique< ObjectIdentity >( *this );
}

const std::string&
ObjectIdentity::captain() const
{
  return m_captain;
}

void
ObjectIdentity::serialize_behavior( Serializer& serializer ) const
{
  serializer.push_back( m_captain );
}

void
ObjectIdentity::deserialize_behavior( Deserializer& deserializer )
{
  m_captain = deserializer.pop_front< std::string >();
}

}

