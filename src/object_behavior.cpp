#include <yarrr/object_behavior.hpp>
#include <yarrr/object.hpp>
#include <yarrr/bitmagic.hpp>

namespace
{

  yarrr::ObjectBehavior::Id last_behavior_id{ 0 };
  yarrr::ObjectBehavior::Id
  generate_next_behavior_id()
  {
    return last_behavior_id++;
  }

}

namespace yarrr
{

ObjectBehavior::ObjectBehavior( int synchronization_period )
  : m_id( generate_next_behavior_id() )
  , m_object( nullptr )
  , m_synchronization_period( synchronization_period )
  , m_synchronization_index( 0 )
{
}

ObjectBehavior::ObjectBehavior( int synchronization_period, const Id& id )
  : m_id( id )
  , m_object( nullptr )
  , m_synchronization_period( synchronization_period )
  , m_synchronization_index( 0 )
{
}

const ObjectBehavior::Id&
ObjectBehavior::id() const
{
  return m_id;
}

void
ObjectBehavior::register_to( Object& owner )
{
  m_object = &owner;
  do_register_to( owner );
  owner.components.register_polymorphic_component( *this );
}

void
ObjectBehavior::do_serialize( Serializer& serializer ) const
{
  serializer.push_back( m_id );
  serialize_behavior( serializer );
}

void
ObjectBehavior::do_deserialize( Deserializer& deserializer )
{
  m_id = deserializer.pop_front< Id >();
  deserialize_behavior( deserializer );
}

void
ObjectBehavior::force_synchronization()
{
  m_synchronization_index = 0;
}

bool
ObjectBehavior::should_synchronize()
{
  if ( m_synchronization_period == do_not_synchronize() )
  {
    return false;
  }

  const bool should_synchronize( 0 == m_synchronization_index % m_synchronization_period );
  ++m_synchronization_index;
  return should_synchronize;
}

}

