#include <yarrr/event.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestEvent : public yarrr::Event
  {
    public:
      add_polymorphic_ctci( "test_event" );

  };
}

Describe(an_event)
{
  It( has_polymorphic_ctci )
  {
    AssertThat( event_base.ctci, !Equals( base_referenced_test_event.polymorphic_ctci() ) );
    AssertThat( test_event.ctci, Equals( base_referenced_test_event.polymorphic_ctci() ) );
  }

  yarrr::Event event_base;
  TestEvent test_event;
  yarrr::Event& base_referenced_test_event{ test_event };
};

