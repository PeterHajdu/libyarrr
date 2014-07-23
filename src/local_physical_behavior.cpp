#include <yarrr/local_physical_behavior.hpp>

namespace yarrr
{

void
LocalPhysicalBehavior::register_to(
    the::ctci::Dispatcher&,
    the::ctci::ComponentRegistry& registry )
{
  registry.register_component< LocalPhysicalBehavior >( *this );
}

}

