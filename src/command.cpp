#include <yarrr/command.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>

namespace
{
  yarrr::AutoEntityRegister<yarrr::Command> register_command;

  std::string
  first( const yarrr::Command::StringTokens& tokens )
  {
    if ( tokens.empty() )
    {
      return "";
    }

    return *std::begin( tokens );
  }

  yarrr::Command::StringTokens
  all_from_second( const yarrr::Command::StringTokens& tokens )
  {
    if ( tokens.empty() )
    {
      return yarrr::Command::StringTokens{};
    }

    yarrr::Command::StringTokens parameters( std::begin( tokens ) + 1, std::end( tokens ) );
    return parameters;
  }
}

namespace yarrr
{

Command::Command( const StringTokens& strings )
  : m_command( first( strings ) )
  , m_parameters( all_from_second( strings ) )
{
}

const std::string&
Command::command() const
{
  return m_command;
}

const Command::StringTokens&
Command::parameters() const
{
  return m_parameters;
}

void
Command::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_command );
  const uint8_t number_of_parameters( m_parameters.size() );
  serializer.push_back( number_of_parameters );
  for ( const auto& parameter : m_parameters )
  {
    serializer.push_back( parameter );
  }
}

void
Command::do_deserialize( Deserializer& deserializer )
{
  m_command = deserializer.pop_front< std::string >();
  const uint8_t number_of_parameters( deserializer.pop_front< uint8_t >() );
  for ( uint8_t i( 0 ); i < number_of_parameters; ++i )
  {
    m_parameters.push_back( deserializer.pop_front< std::string >() );
  }
}

}

