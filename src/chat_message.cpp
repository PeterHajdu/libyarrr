#include <yarrr/chat_message.hpp>
#include <yarrr/event_factory.hpp>

namespace
{
  yarrr::AutoEventRegister<yarrr::ChatMessage> register_chat_message;
}

yarrr::ChatMessage::ChatMessage()
{
}

