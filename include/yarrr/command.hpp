#pragma once

#include <yarrr/entity.hpp>
#include <thectci/id.hpp>
#include <string>
#include <vector>

namespace yarrr
{

class Command : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_command" )
    typedef std::vector< std::string > StringTokens;
    Command() = default;
    Command( const StringTokens& strings );

    const std::string& command() const;
    const StringTokens& parameters() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    std::string m_command;
    StringTokens m_parameters;
};

}

