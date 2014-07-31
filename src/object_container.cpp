#include <yarrr/object_container.hpp>

namespace yarrr
{


void
ObjectContainer::add_object( Object::Id id, Object::Pointer&& object )
{
  register_dispatcher( *object );
  m_objects.emplace( id, std::move( object ) );
}


void
ObjectContainer::delete_object( Object::Id id )
{
  Objects::iterator object( m_objects.find( id ) );
  if ( object == m_objects.end() )
  {
    return;
  }

  remove_dispatcher( *object->second );
  m_objects.erase( object );
}


}

