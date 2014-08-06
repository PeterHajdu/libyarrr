#include <yarrr/object_container.hpp>

namespace yarrr
{


void
ObjectContainer::add_object( Object::Pointer&& object )
{
  register_dispatcher( *object );
  m_objects.emplace( object->id, std::move( object ) );
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

std::vector< ObjectUpdate::Pointer >
ObjectContainer::generate_object_updates() const
{
  std::vector< ObjectUpdate::Pointer > updates;
  for ( const auto& object : m_objects )
  {
    updates.emplace_back( object.second->generate_update() );
  }
  return updates;
}

void
ObjectContainer::handle_object_update( const ObjectUpdate& update )
{
  if ( has_object_with_id( update.id() ) )
  {
    update.update_object( object_with_id( update.id() ) );
    return;
  }

  add_object( update.create_object() );
}

}

