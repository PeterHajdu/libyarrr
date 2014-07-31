#pragma once

#include <yarrr/entity.hpp>
#include <yarrr/bitmagic.hpp>
#include <thectci/id.hpp>
#include <string>

namespace yarrr
{

class ChatMessage : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_chat_message" );

    ChatMessage() = default;
    ChatMessage( const std::string& message, const std::string& sender );
    const std::string& message() const;
    const std::string& sender() const;

  private:
    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;

    std::string m_message;
    std::string m_sender;
};

}

