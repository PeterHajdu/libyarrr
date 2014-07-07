#include <yarrr/event_factory.hpp>
#include <yarrr/event.hpp>
#include <yarrr/bitmagic.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{

class TestEvent : public yarrr::Event
{
  public:
    int data;

    TestEvent()
      : data( 0 )
    {
    }

    TestEvent( int data )
      : data( data )
    {
    }

    add_polymorphic_ctci( "test_event" );

  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override
    {
      serializer.push_back( data );
    }

    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override
    {
      data = deserializer.pop_front<int>();
    }
};

}

Describe(the_event_factory)
{
  void SetUp()
  {
    yarrr::EventFactory::destroy();
    yarrr::EventFactory::register_class<TestEvent>();
    serialized_event = TestEvent( test_data ).serialize();
    event = yarrr::EventFactory::create( serialized_event );
  }

  It( creates_instances_of_a_registered_class )
  {
    AssertThat( event->polymorphic_ctci(), Equals( TestEvent::ctci ) );
  }

  It( deserializes_the_class )
  {
    const TestEvent& test_event( static_cast<const TestEvent&>( *event ) );
    AssertThat( test_event.data, Equals( test_data ) );
  }

  int test_data{ 13 };
  yarrr::Data serialized_event;
  yarrr::Event::Pointer event;
};

Describe(an_auto_register)
{
  void SetUp()
  {
    yarrr::EventFactory::destroy();
  }

  It( registers_event_class_to_event_factory )
  {
    yarrr::AutoEventRegister<TestEvent>();
    yarrr::Event::Pointer event(
        yarrr::EventFactory::create(
          TestEvent().serialize() ) );
    AssertThat( event->polymorphic_ctci(), Equals( TestEvent::ctci ) );
  }
};

