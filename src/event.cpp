#include <yarrr/event.hpp>
#include <yarrr/bitmagic.hpp>
#include <cassert>

namespace yarrr
{

Data
Event::serialize() const
{
  Data buffer;
  do_serialize( Serializer( buffer ).push_back( polymorphic_ctci() ) );
  return buffer;
}

void
Event::deserialize( const Data& data )
{
  Deserializer deserializer( data );
  assert( deserializer.pop_front<the::ctci::Id>() == polymorphic_ctci() );
  do_deserialize( deserializer );
}

}

