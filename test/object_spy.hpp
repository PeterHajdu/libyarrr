#pragma once

#include <yarrr/object.hpp>

namespace test
{


class ObjectSpy : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "test_object_spy" );
    ObjectSpy()
      : dispatcher( nullptr )
      , components( nullptr )
    {
    }

    void register_to(
        the::ctci::Dispatcher& object_dispatcher,
        the::ctci::ComponentRegistry& object_components ) override
    {
      dispatcher = &object_dispatcher;
      components = &object_components;
    }

    Pointer clone() const
    {
      return Pointer( new ObjectSpy() );
    }

    the::ctci::Dispatcher* dispatcher;
    the::ctci::ComponentRegistry* components;
};


ObjectSpy* spy_on( yarrr::Object& object )
{
  std::unique_ptr< ObjectSpy > new_spy( new ObjectSpy() );
  ObjectSpy* spy_ptr( new_spy.get() );
  object.add_behavior( yarrr::ObjectBehavior::Pointer( new_spy.release() ) );
  return spy_ptr;
}

}

