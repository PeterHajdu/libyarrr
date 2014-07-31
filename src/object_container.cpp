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


bool
ObjectContainer::has_object_with_id( Object::Id id ) const
{
  return m_objects.find( id ) != m_objects.end();
}


const Object&
ObjectContainer::object_with_id( Object::Id id ) const
{
  Objects::const_iterator object( m_objects.find( id ) );
  assert( object != m_objects.end() );
  return *object->second;
}

size_t
ObjectContainer::size() const
{
  return m_objects.size();
}

}

