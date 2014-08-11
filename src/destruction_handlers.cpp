#include <yarrr/destruction_handlers.hpp>

namespace yarrr
{

void
DeleteWhenDestroyed::register_to( Object& owner )
{
  owner.components.register_component( *this );
}

ObjectBehavior::Pointer
DeleteWhenDestroyed::clone() const
{
  return Pointer( new DeleteWhenDestroyed() );
}

}

