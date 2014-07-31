#include <yarrr/entity.hpp>
#include <yarrr/bitmagic.hpp>
#include <cassert>

namespace yarrr
{

Data
Entity::serialize() const
{
  Data buffer;
  do_serialize( Serializer( buffer ).push_back( polymorphic_ctci() ) );
  return buffer;
}

void
Entity::deserialize( const Data& data )
{
  Deserializer deserializer( data );
  assert( deserializer.pop_front<the::ctci::Id>() == polymorphic_ctci() );
  do_deserialize( deserializer );
}

}

