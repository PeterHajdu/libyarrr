#include <yarrr/callback_queue.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_callback_queue)
{
  void SetUp()
  {
    test_queue.process_callbacks();
  }

  It( accepts_lambdas )
  {
    bool it_was_called( false );
    test_queue.push_back(
        [ &it_was_called ] ()
        {
          it_was_called = true;
        } );
    AssertThat( it_was_called, Equals( false ) );
    test_queue.process_callbacks();
    AssertThat( it_was_called, Equals( true ) );
  }

  It( can_hold_many_callbacks )
  {
    const size_t number_of_callbacks( 5 );
    size_t number_of_calls( 0 );
    for ( size_t i( 0 ); i < number_of_callbacks; ++i )
    {
      test_queue.push_back(
          [ &number_of_calls ]()
          {
            ++number_of_calls;
          } );
    }
    test_queue.process_callbacks();
    AssertThat( number_of_calls, Equals( number_of_callbacks ) );
  }

  yarrr::CallbackQueue test_queue;
};

