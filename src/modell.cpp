#include <yarrr/modell.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/log.hpp>

namespace
{

yarrr::AutoEntityRegister<yarrr::ModellSerializer> register_modell_serializer;

}

namespace yarrr
{

ModellSerializer::ModellSerializer()
{
}

ModellSerializer::ModellSerializer( const Hash& hash )
{
  thelog( log::debug )( "Initializing modell serializer from hash." );
  for ( auto iterator( hash.cbegin() ); iterator != hash.cend(); ++iterator )
  {
    const auto key( iterator->first );
    const auto value( static_cast< std::string >( *iterator->second ) );
    thelog( log::debug )( "key:", key, "value:", value );
    m_hash[ key ] = value;;
  }
}

std::unique_ptr< Hash >
ModellSerializer::update_parent( the::model::OwningNode& parent ) const
{
  auto hash( std::make_unique< Hash >( m_hash.at( "id" ), parent ) );
  for ( const auto& key_value : m_hash )
  {
    ( *hash )[ key_value.first ] = key_value.second;
  }
  return hash;
}

void
ModellSerializer::do_serialize( Serializer& serializer ) const
{
  thelog( log::debug )( "Serializing modell serializer." );
  for ( const auto& key_value : m_hash )
  {
    serializer.push_back( key_value.first );
    serializer.push_back( key_value.second );
    thelog( log::debug )( "key:", key_value.first, "value:", key_value.second );
  }
}

void
ModellSerializer::do_deserialize( Deserializer& deserializer )
{
  thelog( log::debug )( "Deserializing modell serializer." );
  while ( deserializer.bytes_left() )
  {
    const std::string key{ deserializer.pop_front< std::string >() };
    const std::string value{ deserializer.pop_front< std::string >() };
    m_hash.emplace( std::make_pair( key, value ) );
    thelog( log::debug )( "key:", key, "value:", value );
  }
}

ModellSerializer::~ModellSerializer() = default;

std::string
ModellSerializer::id() const
{
  return retrieve_value( "id" );
}

std::string
ModellSerializer::category() const
{
  return retrieve_value( "category" );
}

std::string
ModellSerializer::retrieve_value( const std::string& key ) const
{
  const auto iterator( m_hash.find( key ) );

  return
    iterator == std::end( m_hash ) ?
    "" :
    iterator->second;
}

}

