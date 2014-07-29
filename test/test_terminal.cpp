#include <yarrr/terminal.hpp>
#include <yarrr/chat_message.hpp>
#include <thectci/dispatcher.hpp>
#include "test_graphical_engine.hpp"
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  std::string formatted_chat_message( const yarrr::ChatMessage& chat_message )
  {
    return chat_message.sender() + ": " + chat_message.message();
  }
}

Describe(a_terminal)
{
  void SetUp()
  {
    test_engine.reset( new test::GraphicalEngine() );
    test_terminal.reset( new yarrr::Terminal( *test_engine, n ) );
    test_engine->printed_texts.clear();
  }

  It( prints_out_chat_messages )
  {
    test_terminal->dispatch( test_chat_message );
    test_engine->draw_objects();
    AssertThat( test_engine->last_printed_text, Contains( test_chat_message.message() ) );
  }

  It( prints_out_sender_of_chat_message )
  {
    test_terminal->dispatch( test_chat_message );
    test_engine->draw_objects();
    AssertThat( test_engine->last_printed_text, Contains( test_chat_message.sender() + ":" ) );
  }

  It( prints_out_only_the_last_n_chat_message )
  {
    for ( int i( 0 ); i < n + 1; ++i )
    {
      test_terminal->dispatch( test_chat_message );
    }
    const yarrr::ChatMessage last_message( "last message", "" );
    test_terminal->dispatch( last_message );
    test_engine->draw_objects();

    AssertThat( test_engine->printed_texts, Has().Exactly( n - 1 ).EqualTo( formatted_chat_message( test_chat_message ) ) );
    AssertThat( test_engine->printed_texts.back(), Equals( formatted_chat_message( last_message ) ) );
  }

  const int n{ 3 };
  const yarrr::ChatMessage test_chat_message{ "a test message", "Kilgore Trout" };
  std::unique_ptr< test::GraphicalEngine > test_engine;
  std::unique_ptr< yarrr::Terminal > test_terminal;
};

