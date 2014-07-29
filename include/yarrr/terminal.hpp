#pragma once

#include <yarrr/graphical_engine.hpp>
#include <vector>
#include <string>

namespace the
{
namespace ctci
{

class Dispatcher;

}
}

namespace yarrr
{

class ChatMessage;

class Terminal : public GraphicalObject
{
  public:
    Terminal( GraphicalEngine&, the::ctci::Dispatcher&, size_t number_of_messages );
    virtual void draw() const override;

  private:
    void handle_chat_message( const ChatMessage& );
    std::vector< std::string > m_messages;
    const size_t m_number_of_shown_messages;
};

}

