#pragma once

#include <thectci/id.hpp>

namespace test
{

class Event
{
  public:
    add_polymorphic_ctci( "test_event" );
};

class EventChild : public Event
{
  public:
    add_polymorphic_ctci( "test_event_child" );
};

}

