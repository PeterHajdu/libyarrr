#pragma once
#include "test_events.hpp"
#include <yarrr/object.hpp>

namespace test
{

class Behavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_another_test_behavior" );
    Behavior( std::function< void() > call_when_deleted )
      : m_call_when_deleted( call_when_deleted )
    {
    }

    virtual void register_to( yarrr::Object& owner )
    {
      owner.dispatcher.register_listener< test::Event >( std::bind(
            &Behavior::handle_event, this, std::placeholders::_1 ) );
      owner.dispatcher.register_listener< Behavior >( std::bind(
            &Behavior::handle_test_behavior, this, std::placeholders::_1 ) );
    }

    const test::Event* dispatched_event{ nullptr };
    void handle_event( const test::Event& event )
    {
      dispatched_event = &event;
    }

    virtual ~Behavior()
    {
      m_call_when_deleted();
    }

    bool was_object_updated{ false };
    void handle_test_behavior( const Behavior& )
    {
      was_object_updated = true;
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      return yarrr::ObjectBehavior::Pointer( new Behavior( m_call_when_deleted ) );
    }

  private:
    std::function< void() > m_call_when_deleted;
};

}

