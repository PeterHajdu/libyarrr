#include <yarrr/command.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/entity_factory.hpp>

namespace
{
  yarrr::AutoEntityRegister<yarrr::Command> register_command;
}

namespace yarrr
{


Command::Command( Type type, the::time::Time timestamp )
  : m_type( type )
  , m_timestamp( timestamp )
{
}


Command::Type
Command::type() const
{
  return m_type;
}


const the::time::Time&
Command::timestamp() const
{
  return m_timestamp;
}


void
Command::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_type );
  serializer.push_back( m_timestamp );
}


void
Command::do_deserialize( Deserializer& deserializer )
{
  m_type = deserializer.pop_front<Type>();
  m_timestamp = deserializer.pop_front<the::time::Time>();
}

}

