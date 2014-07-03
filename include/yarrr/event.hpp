#pragma once

#include <thectci/id.hpp>

namespace yarrr
{

class Event
{
  public:
    add_polymorphic_ctci( "yarrr_event" );
    virtual ~Event() = default;
};

}

