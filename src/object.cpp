#include <yarrr/object.hpp>

namespace yarrr
{

Object::Object()
  : id( Id( this ) )
{
}

Object::Object( const Id& id )
  : id( id )
{
}

void
Object::add_behavior( ObjectBehavior::Pointer&& behavior )
{
  behavior->register_to( *this, m_components );
  m_behaviors.emplace_back( std::move( behavior ) );
}

ObjectUpdate
Object::generate_update() const
{
  return ObjectUpdate( id );
}

ObjectUpdate::ObjectUpdate( const Object::Id& id )
  : id( id )
{
}

}

