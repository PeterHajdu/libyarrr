#pragma once
#include <thectci/id.hpp>
#include <string>
#include <vector>

namespace yarrr
{

class Db
{
  public:
    add_ctci( "yarrr_db" );
    virtual ~Db() = default;

    virtual bool set_hash_field(
        const std::string& key,
        const std::string& field,
        const std::string& value ) = 0;

    virtual bool get_hash_field(
        const std::string& key,
        const std::string& field,
        std::string& value ) = 0;

    virtual bool add_to_set(
        const std::string& key,
        const std::string& value ) = 0;

    virtual bool key_exists( const std::string& key ) = 0;

    using Values = std::vector< std::string >;
    virtual bool get_set_members(
        const std::string& key,
        Values& ) = 0;

    virtual bool get_hash_fields(
        const std::string& key,
        Values& ) = 0;
};

std::string
player_key_from_id( const std::string& player_id );

}

