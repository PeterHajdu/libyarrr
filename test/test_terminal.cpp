#include <yarrr/terminal.hpp>
#include <yarrr/chat_message.hpp>
#include <thectci/dispatcher.hpp>
#include "test_graphical_engine.hpp"
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe(a_terminal)
{
  void SetUp()
  {
    test_engine.reset( new test::GraphicalEngine() );
    test_dispatcher.reset( new the::ctci::Dispatcher() );
    test_terminal.reset( new yarrr::Terminal( *test_engine, *test_dispatcher, n ) );
    test_engine->printed_texts.clear();
  }

  It( prints_out_chat_messages )
  {
    test_dispatcher->dispatch( test_chat_message );
    test_engine->draw_objects();
    AssertThat( test_engine->printed_texts, Contains( test_chat_message.message() ) );
  }

  It( prints_out_only_the_last_n_chat_message )
  {
    for ( int i( 0 ); i < n + 1; ++i )
    {
      test_dispatcher->dispatch( test_chat_message );
    }
    test_engine->draw_objects();

    AssertThat( test_engine->printed_texts, Has().Exactly( n ).EqualTo( test_chat_message.message() ) );
  }

  const int n{ 3 };
  const yarrr::ChatMessage test_chat_message{ "a test message" };
  std::unique_ptr< test::GraphicalEngine > test_engine;
  std::unique_ptr< the::ctci::Dispatcher > test_dispatcher;
  std::unique_ptr< yarrr::Terminal > test_terminal;
};

