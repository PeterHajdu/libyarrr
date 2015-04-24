#include <yarrr/modell.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/id_generator.hpp>
#include <yarrr/test_db.hpp>
#include <themodel/lua.hpp>
#include <themodel/node.hpp>
#include <themodel/json_exporter.hpp>

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

    yarrr::Hash deserialized_hash( deserialized_serializer.id(), *remote );
    deserialized_serializer.update_hash( deserialized_hash );
    AssertThat( deserialized_hash.get( key ), Equals( value ) );
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


Describe( a_modell_container )
{
  void create_some_hashes()
  {
    const int number_of_hashes{ 5 };
    character_ids.clear();
    for ( auto i( 0 ); i < number_of_hashes; ++i )
    {
      character_ids.push_back( modell_container->create( category ).get( "id" ) );
    }
  }

  void reset_modells_while_keeping_database()
  {
    modell_container.reset();
    lua = std::make_unique< the::model::Lua >();
    modell_container = std::make_unique< yarrr::ModellContainer >( *lua, id_generator, *database );
  }

  void SetUp()
  {
    database = std::make_unique< test::Db >();
    reset_modells_while_keeping_database();
  }

  void assert_modell_exists( const std::string& category, const std::string& id )
  {
    const auto path(
        the::model::path_from( { "modells", category, id, "id" } ) );
    AssertThat(
        lua->assert_equals( path, id ),
        Equals( true ) );
  }

  It( creates_hashes_with_category )
  {
    const auto& a_character = modell_container->create( "character" );
  }

  It( creates_hashes_with_id_and_category )
  {
    auto& a_character = modell_container->create( category );
    AssertThat( a_character.has( "id" ), Equals( true ) );
    AssertThat( a_character.has( "category" ), Equals( true ) );
    AssertThat( a_character[ "category" ].get(), Equals( category ) );
  }

  It( exports_hashes_under_modells )
  {
    auto& a_character = modell_container->create( category );
    assert_modell_exists( category, a_character.get( "id" ) );
  }

  It( contains_many_hashes )
  {
    create_some_hashes();
    for ( const auto& id : character_ids )
    {
      assert_modell_exists( category, id );
    }
  }

  It( allows_iteration_on_a_category )
  {
    create_some_hashes();
    const auto& characters( modell_container->get( category ) );
    AssertThat( characters, HasLength( character_ids.size() ) );
    for ( const auto& character_model : characters )
    {
      AssertThat( character_ids, Contains( character_model.first ) );
    }
  }

  It( creates_hashes_from_modell_serializer )
  {
    const auto id( "something" );
    yarrr::Hash a_hash( id, *lua );
    a_hash[ "id" ] = id;
    a_hash[ "category" ] = category;

    const std::string another_value( "another value" );
    const std::string another_member( "another_member" );
    a_hash[ another_member ] = another_value;

    modell_container->update( yarrr::ModellSerializer( a_hash ) );
    assert_modell_exists( category, id );

    AssertThat(
        lua->assert_equals( the::model::path_from(
            { "modells", category, id, another_member } ),
          another_value ),
        Equals( true ) );
  }

  It( saves_modell_data_to_the_database_automatically )
  {
    create_some_hashes();
    for ( const auto& id : character_ids )
    {
      AssertThat(
          database->does_set_contain( category, id ),
          Equals( true ) );
    }
  }

  It( saves_modell_hashes_to_the_database_automatically )
  {
    create_some_hashes();
    for ( const auto& id : character_ids )
    {
      std::string value;
      AssertThat(
          database->get_hash_field( category + ":" + id, "id", value ),
          Equals( true ) );
      AssertThat(
          value,
          Equals( id ) );
    }
  }

  It( synchronizes_hashes_automatically_with_the_database )
  {
    auto& a_character = modell_container->create( category );
    const std::string new_key{ "new_data_key" };
    const std::string new_value{ "new data value" };
    a_character[ new_key ] = new_value;

    std::string value_in_database;
    AssertThat(
        database->get_hash_field( category + ":" + a_character.get( "id" ), new_key, value_in_database ),
        Equals( true ) );
    AssertThat(
        value_in_database,
        Equals( new_value ) );
  }

  It( saves_known_category_names_to_a_set_in_the_database )
  {
    create_some_hashes();
    AssertThat(
        database->does_set_contain( "categories", category ),
        Equals( true ) );
  }

  It( initializes_modells_from_the_database_when_created )
  {
    auto& a_character = modell_container->create( category );
    const std::string id{ a_character[ "id" ] };
    const std::string new_key{ "new_data_key" };
    const std::string new_value{ "new data value" };
    a_character[ new_key ] = new_value;

    reset_modells_while_keeping_database();
    assert_modell_exists( category, id );
    AssertThat(
        lua->assert_equals( the::model::path_from(
            { "modells", category, id, new_key } ),
          new_value ),
        Equals( true ) );
  }

  It( can_tell_if_a_modell_already_exists )
  {
    create_some_hashes();
    AssertThat( modell_container->exists( "dummy category", "dummy id" ), Equals( false ) );

    const auto& existing_category( category );
    AssertThat( modell_container->exists( existing_category, "dummy id" ), Equals( false ) );

    const auto& existing_id( character_ids.back() );
    AssertThat( modell_container->exists( existing_category, existing_id ), Equals( true ) );
  }

  std::unique_ptr< test::Db > database;
  std::vector< std::string > character_ids;
  const std::string category{ "character" };
  yarrr::IdGenerator id_generator;
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< yarrr::ModellContainer > modell_container;
};

