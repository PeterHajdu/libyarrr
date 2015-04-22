#pragma once

#include <yarrr/entity.hpp>
#include <themodel/hash.hpp>

#include <unordered_map>

namespace yarrr
{
class Serializer;
class Deserializer;

using Hash = the::model::Hash< std::string, std::string >;

class ModellSerializer : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_modell_serializer" );
    ModellSerializer( const Hash& );
    ModellSerializer();
    virtual ~ModellSerializer();
    std::unique_ptr< Hash > update_parent( the::model::OwningNode& ) const;
    std::string id() const;
    std::string category() const;

  private:
    std::string retrieve_value( const std::string& key ) const;
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    std::unordered_map< std::string, std::string > m_hash;
};

}

