#include <yarrr/modell.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/log.hpp>
#include <yarrr/id_generator.hpp>
#include <yarrr/db.hpp>

namespace
{

yarrr::AutoEntityRegister<yarrr::ModellSerializer> register_modell_serializer;
const std::string categories_key{ "categories" };

std::string
modell_key_from( const std::string& category, const std::string& id )
{
  return category + ":" + id;
}

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
    const auto value( iterator->second->get() );
    thelog( log::debug )( "key:", key, "value:", value );
    m_hash[ key ] = value;;
  }
}

void
ModellSerializer::update_hash( Hash& hash ) const
{
  for ( const auto& key_value : m_hash )
  {
    hash[ key_value.first ] = key_value.second;
  }
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
  return retrieve_value( model::id );
}

std::string
ModellSerializer::category() const
{
  return retrieve_value( model::category );
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


ModellContainer::CategoryNode&
ModellContainer::category_parent( const std::string& category_name )
{
  auto category_node( m_categories.find( category_name ) );
  if ( category_node == std::end( m_categories ) )
  {
    bool was_created{ false };
    std::tie( category_node, was_created ) = m_categories.insert( std::make_pair(
          category_name,
          std::make_unique< CategoryNode >( category_name, m_container_modell ) ) );
  }

  return *category_node->second;
}


Hash&
ModellContainer::create( const std::string& category )
{
  const auto id( category + "_" + m_id_generator.generate( category ) );
  return create_with_id_if_needed( category, id );
}


const ModellContainer::HashContainer&
ModellContainer::get( const std::string& category )
{
  return category_container( category );
}


ModellContainer::HashContainer&
ModellContainer::category_container( const std::string& category_name )
{
  auto category_hashes( m_hashes.find( category_name ) );
  if ( category_hashes == std::end( m_hashes ) )
  {
    bool was_created{ false };
    std::tie( category_hashes, was_created ) = m_hashes.insert( std::make_pair(
          category_name,
          std::make_unique< HashContainer >() ) );

    m_database.add_to_set( "categories", category_name );
  }

  return *category_hashes->second;
}


Hash&
ModellContainer::create_with_id_if_needed( const std::string& category, const std::string& id )
{
  auto& container( category_container( category ) );
  auto modell_iterator( container.find( id ) );
  if ( modell_iterator != std::end( container ) )
  {
    return *modell_iterator->second;
  }

  bool was_emplace{ false };
  std::tie( modell_iterator, was_emplace ) =
    container.emplace( std::make_pair(
          id,
          std::make_unique< Hash >( id, category_parent( category ) ) ) );
  m_database.add_to_set( category, id );
  m_database.set_hash_field( modell_key_from( category, id ), model::id, id );

  auto& hash_reference( *modell_iterator->second );
  hash_reference[ model::id ] = id;
  hash_reference[ model::category ] = category;

  add_observer( hash_reference );

  return hash_reference;
}

void
ModellContainer::add_observer( Hash& hash )
{
  hash.observe(
      [ &db = m_database ]( const yarrr::Hash& h )
      {
        const std::string key{ modell_key_from( h.get( model::category ), h.get( model::id ) ) };
        for ( auto i( h.cbegin() ); i!=h.cend(); ++i )
        {
          const auto field( i->first );
          const auto value( i->second->get() );
          db.set_hash_field( key, field, value );
        }
      } );
}


bool
ModellContainer::exists( const std::string& category, const std::string& id ) const
{
  auto category_hashes( m_hashes.find( category ) );
  if ( category_hashes == std::end( m_hashes ) )
  {
    return false;
  }

  auto& container( *category_hashes->second );
  auto modell_iterator( container.find( id ) );
  if ( modell_iterator == std::end( container ) )
  {
    return false;
  }

  return true;
}


void
ModellContainer::update( const ModellSerializer& modell )
{
  auto& stored_modell( create_with_id_if_needed( modell.category(), modell.id() ) );
  modell.update_hash( stored_modell );
}


void
ModellContainer::initialize_from_database()
{
  Db::Values categories_in_db;
  m_database.get_set_members( categories_key, categories_in_db );

  for ( const auto& category : categories_in_db )
  {
    initialize_category( category );
  }

}

void
ModellContainer::initialize_category( const std::string& category )
{
  category_parent( category );
  Db::Values ids_in_category;
  m_database.get_set_members( category, ids_in_category );
  for ( const auto& id : ids_in_category )
  {
    initialize_hash( category, id );
  }
}

void
ModellContainer::initialize_hash( const std::string& category, const std::string& id  )
{
  auto& hash( create_with_id_if_needed( category, id ) );
  const auto hash_key( modell_key_from( category, id ) );
  Db::Values fields_in_hash;
  m_database.get_hash_fields( hash_key, fields_in_hash );
  for ( const auto& field : fields_in_hash )
  {
    std::string value;
    if ( !m_database.get_hash_field( hash_key, field, value ) )
    {
      continue;
    }

    hash[ field ] = value;
  }
}

}

