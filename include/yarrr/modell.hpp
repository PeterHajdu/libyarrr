#pragma once

#include <yarrr/entity.hpp>
#include <themodel/hash.hpp>

#include <unordered_map>

namespace yarrr
{
class Serializer;
class Deserializer;
class IdGenerator;
class Db;

using Hash = the::model::Hash< std::string, std::string >;

class ModellSerializer : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_modell_serializer" );
    ModellSerializer( const Hash& );
    ModellSerializer();
    virtual ~ModellSerializer();
    void update_hash( Hash& ) const;
    std::string id() const;
    std::string category() const;

  private:
    std::string retrieve_value( const std::string& key ) const;
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    std::unordered_map< std::string, std::string > m_hash;
};

class ModellContainer
{
  public:
    add_ctci( "yarrr_modell_container" );

    template < typename Parent >
    ModellContainer( Parent& parent, IdGenerator& id_generator, Db& database )
      : m_database( database )
      , m_id_generator( id_generator )
      , m_container_modell( "modells", parent )
    {
      initialize_from_database();
    }

    Hash& create( const std::string& category );
    Hash& create_with_id_if_needed( const std::string& category, const std::string& id );

    bool exists( const std::string& category, const std::string& id ) const;

    using HashPointer = std::unique_ptr< Hash >;
    using HashContainer = std::unordered_map< std::string, HashPointer >;
    const HashContainer& get( const std::string& category );

    void update( const ModellSerializer& );

  private:
    void initialize_from_database();
    void initialize_category( const std::string& category );
    void initialize_hash( const std::string& category, const std::string& id  );

    void add_observer( Hash& );

    using CategoryNode = the::model::OwningNode;
    CategoryNode& category_parent( const std::string& );

    HashContainer& category_container( const std::string& );

    Db& m_database;
    IdGenerator& m_id_generator;
    the::model::OwningNode m_container_modell;

    using CategoryNodePointer = std::unique_ptr< CategoryNode >;
    using CategoryContainer = std::unordered_map< std::string, CategoryNodePointer >;
    CategoryContainer m_categories;

    using HashContainerPointer = std::unique_ptr< HashContainer >;
    using HashContainerContainer = std::unordered_map< std::string, HashContainerPointer >;
    HashContainerContainer m_hashes;

};

namespace model
{

using key_type = std::string;
const key_type id{ "_id" };
const key_type category{ "_category" };
const key_type object_id{ "_object_id" };
const key_type realtime_object_id{ "_realtime_object_id" };
const key_type character_id{ "_character_id" };
const key_type object{ "_object" };
const key_type character{ "_character" };
const key_type player{ "_player" };
const key_type availability{ "_availability" };
const key_type name{ "name" };
const key_type object_type{ "_object_type" };
const key_type player_controlled{ "player_controlled" };
const key_type ship_type{ "_ship_type" };
const key_type hidden_x{ "_x" };
const key_type hidden_y{ "_y" };
const key_type hidden_angular_velocity{ "_angular_velocity" };
const key_type auth_token{ "!auth_token" };
const key_type characters_onboard{ "_characters_onboard" };
const key_type in_dock_of{ "_in_dock_of" };

}

}

