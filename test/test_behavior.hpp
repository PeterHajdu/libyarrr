#pragma once
#include "test_events.hpp"
#include <yarrr/object.hpp>

namespace test
{

class Behavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_another_test_behavior" );
    Behavior( std::function< void() > call_when_deleted = [](){} )
      : m_call_when_deleted( call_when_deleted )
    {
    }

    size_t number_of_test_behavior_registrations{ 1 };
    bool was_registered{ false };
    virtual void register_to( yarrr::Object& owner )
    {
      owner.dispatcher.register_listener< test::Event >( std::bind(
            &Behavior::handle_event, this, std::placeholders::_1 ) );
      owner.dispatcher.register_listener< Behavior >( std::bind(
            &Behavior::handle_test_behavior, this, std::placeholders::_1 ) );
      was_registered = true;

      const bool was_already_registered( owner.components.has_component< Behavior >() );
      if ( was_already_registered )
      {
        number_of_test_behavior_registrations =
          ++owner.components.component< Behavior >().number_of_test_behavior_registrations;
      }
      owner.components.register_component( *this );
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
    size_t number_of_behaviors_dispatched{ 0 };
    void handle_test_behavior( const Behavior& )
    {
      was_object_updated = true;
      ++number_of_behaviors_dispatched;
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      return yarrr::ObjectBehavior::Pointer( new Behavior( m_call_when_deleted ) );
    }

  private:
    std::function< void() > m_call_when_deleted;
};

}

