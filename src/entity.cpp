#include <yarrr/entity.hpp>
#include <yarrr/bitmagic.hpp>
#include <cassert>

namespace yarrr
{

void
Entity::serialize( Serializer& serializer ) const
{
  serializer.push_back( polymorphic_ctci() );
  do_serialize( serializer );
}

Data
Entity::serialize() const
{
  Data buffer;
  Serializer serializer( buffer );
  serialize( serializer );
  return buffer;
}


void
Entity::deserialize( Deserializer& deserializer )
{
  assert( deserializer.pop_front<the::ctci::Id>() == polymorphic_ctci() );
  do_deserialize( deserializer );
}

void
Entity::deserialize( const Data& data )
{
  Deserializer deserializer( data );
  deserialize( deserializer );
}

}

