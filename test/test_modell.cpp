#include <yarrr/modell.hpp>
#include <yarrr/entity_factory.hpp>
#include <themodel/lua.hpp>
#include <themodel/node.hpp>

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_modell_serializer )
{
  void SetUp()
  {
    original.reset();
    remote.reset();

    lua = std::make_unique< the::model::Lua >();
    original = std::make_unique< the::model::OwningNode >( "original", *lua );
    remote = std::make_unique< the::model::OwningNode >( remote_name, *lua );
    hash = std::make_unique< yarrr::Hash >( id_value, *lua );
    (*hash)[ key ] = value;
    (*hash)[ id_key ] = id_value;
    (*hash)[ category_key ] = category_value;

    serializer = std::make_unique< yarrr::ModellSerializer >( *hash );
  }

  It( is_an_entity )
  {
    yarrr::Entity& serializer_as_entity{ *serializer };
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ModellSerializer::ctci ), Equals( true ) );
  }

  It( serializes_and_deserializes_the_original_hash )
  {
    const auto serialized_modell( serializer->serialize() );
    const auto entity( yarrr::EntityFactory::create( serialized_modell ) );
    const auto& deserialized_serializer( static_cast< yarrr::ModellSerializer& >( *entity ) );

    const std::unique_ptr< yarrr::Hash > deserialized_hash( deserialized_serializer.update_parent( *remote ) );
    AssertThat( deserialized_hash.get() == nullptr, !Equals( true ) );
    AssertThat( static_cast< std::string >( ( *deserialized_hash )[ key ] ), Equals( value ) );
    AssertThat(
        lua->assert_equals( the::model::path_from( { remote_name, id_value, key } ), value ),
        Equals( true ) );
  }

  It( allows_direct_access_to_id_value )
  {
    AssertThat( serializer->id(), Equals( id_value ) );
  }

  It( allows_direct_access_to_category_value )
  {
    AssertThat( serializer->category(), Equals( category_value ) );
  }

  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::OwningNode > original;
  std::unique_ptr< the::model::OwningNode > remote;
  std::unique_ptr< yarrr::Hash > hash;
  std::unique_ptr< yarrr::ModellSerializer > serializer;

  const std::string remote_name{ "remote_name" };
  const std::string id_key{ "id" };
  const std::string id_value{ "the_modell_id" };
  const std::string key{ "a_key" };
  const std::string value{ "a_value" };
  const std::string category_key{ "category" };
  const std::string category_value{ "the_modell_category" };
};

