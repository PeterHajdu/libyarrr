#pragma once

#include <yarrr/graphical_engine.hpp>
#include <vector>
#include <string>
#include <thectci/dispatcher.hpp>

namespace yarrr
{

class ChatMessage;

class Terminal : public GraphicalObject, public the::ctci::Dispatcher
{
  public:
    Terminal( GraphicalEngine&, size_t number_of_messages );
    virtual void draw() const override;

  private:
    void handle_chat_message( const ChatMessage& );
    std::vector< TextTokens > m_messages;
    const size_t m_number_of_shown_messages;
};

}

