#pragma once
#include "test_events.hpp"
#include <yarrr/object.hpp>
#include <yarrr/bitmagic.hpp>

namespace test
{

class Behavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_another_test_behavior" );
    Behavior( int synchronization_period = yarrr::always_synchronize(), std::function< void() > call_when_deleted = [](){} )
      : ObjectBehavior( synchronization_period )
      , some_data( id() + 1 )
      , m_call_when_deleted( call_when_deleted )
    {
    }

    Behavior( Id id )
      : ObjectBehavior( yarrr::always_synchronize(), id )
      , some_data( id + 1 )
      , m_call_when_deleted( [](){} )
    {
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

    mutable bool was_cloned{ false };
    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      was_cloned=true;
      return yarrr::ObjectBehavior::Pointer( new Behavior( id() ) );
    }

    const ObjectBehavior* updated_with{ nullptr };
    virtual void update( const ObjectBehavior& behavior ) override
    {
      updated_with = &behavior;
    }

    bool was_updated() const
    {
      return updated_with != nullptr;
    }

    Id some_data;

    bool was_registered{ false };
    static size_t number_of_test_behavior_registrations;
  private:
    virtual void do_register_to( yarrr::Object& owner ) override
    {
      owner.dispatcher.register_listener< test::Event >( std::bind(
            &Behavior::handle_event, this, std::placeholders::_1 ) );
      was_registered = true;
      ++number_of_test_behavior_registrations;
    }

    virtual void serialize_behavior( yarrr::Serializer& serializer ) const override
    {
      serializer.push_back( some_data );
    }

    virtual void deserialize_behavior( yarrr::Deserializer& deserializer ) override
    {
      some_data = deserializer.pop_front< Id >();
    }

    std::function< void() > m_call_when_deleted;
};

class NonSynchronizableBehavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_nonsynchronizable_behavior" );
    NonSynchronizableBehavior()
      : ObjectBehavior( yarrr::do_not_synchronize() )
    {
    }

    mutable bool was_cloned{ false };
    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      was_cloned = true;
      return Pointer{ nullptr };
    }

  private:
    virtual void do_register_to( yarrr::Object& ) override
    {
    }

};

class NthSynchronizedBehavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_nth_synchronized_behavior" );
    NthSynchronizedBehavior( int n )
      : ObjectBehavior( n )
    {
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const
    {
      return Pointer{ nullptr };
    }
};

}

