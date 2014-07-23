#pragma once
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <thectci/id.hpp>

namespace yarrr
{

class LocalPhysicalBehavior : public ObjectBehavior
{
  public:
    add_ctci( "local_physical_behavior" );
    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    PhysicalParameters physical_parameters;
};

}

