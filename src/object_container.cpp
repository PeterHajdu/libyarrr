#include <yarrr/object_container.hpp>
#include <yarrr/collider.hpp>

namespace
{

bool has_collider( const yarrr::Object& object )
{
  return object.components.has_component< yarrr::Collider >();
}

yarrr::Collider& collider_of( const yarrr::Object& object )
{
  return object.components.component< yarrr::Collider >();
}

}

namespace yarrr
{


void
ObjectContainer::add_object( Object::Pointer&& object )
{
  register_dispatcher( object->dispatcher );
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

  remove_dispatcher( object->second->dispatcher );
  m_objects.erase( object );
}


bool
ObjectContainer::has_object_with_id( Object::Id id ) const
{
  return m_objects.find( id ) != m_objects.end();
}


Object&
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

void
ObjectContainer::check_collision() const
{
  typedef Objects::const_iterator Iterator;
  for( Iterator i( std::begin( m_objects ) ); i != std::end( m_objects ); ++i )
  {
    Object& object_1( *i->second );
    if ( !has_collider( object_1 ) )
    {
      continue;
    }

    yarrr::Collider& collider_1( collider_of( object_1 ) );
    for( Iterator j( i ); j != std::end( m_objects ); ++j )
    {
      if ( j == i )
      {
        continue;
      }

      Object& object_2( *j->second );
      if ( !has_collider( object_2 ) )
      {
        continue;
      }

      yarrr::Collider& collider_2( collider_of( object_2 ) );
      collider_1.collide_if_needed_with( collider_2 );
    }
  }
}

}

