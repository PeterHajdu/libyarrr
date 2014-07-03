#include <yarrr/event.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestEvent : public yarrr::Event
  {
    public:
      add_polymorphic_ctci( "test_event" );

    private:
      virtual void do_serialize( yarrr::Serializer& serializer ) const
      {
      }

      virtual void do_deserialize( yarrr::Deserializer& deserializer )
      {
      }

  };
}

Describe(an_event)
{
  It( has_polymorphic_ctci )
  {
    AssertThat( test_event.ctci, Equals( base_referenced_test_event.polymorphic_ctci() ) );
  }

  TestEvent test_event;
  yarrr::Event& base_referenced_test_event{ test_event };
};

