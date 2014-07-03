#include <yarrr/command.hpp>
#include <yarrr/bitmagic.hpp>

namespace yarrr
{


Command::Command( Id id, the::time::Time timestamp )
  : m_id( id )
  , m_timestamp( timestamp )
{
}


Command::Id
Command::id() const
{
  return m_id;
}


const the::time::Time&
Command::timestamp() const
{
  return m_timestamp;
}


void
Command::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_id );
  serializer.push_back( m_timestamp );
}


void
Command::do_deserialize( Deserializer& deserializer )
{
  m_id = deserializer.pop_front<Id>();
  m_timestamp = deserializer.pop_front<the::time::Time>();
}

}

