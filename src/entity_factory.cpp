#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>
#include <thectci/id.hpp>
#include <memory>

namespace
{
  yarrr::EntityFactory* instance;
}

namespace yarrr
{

EntityFactory&
EntityFactory::get()
{
  if ( !instance )
  {
    instance = new yarrr::EntityFactory();
  }

  return *instance;
}


Entity::Pointer
EntityFactory::create( Deserializer& deserializer )
{
  if ( deserializer.bytes_left() < sizeof( the::ctci::Id ) )
  {
    return nullptr;
  }

  EntityFactory& factory( get() );
  Entity::Pointer entity( factory.m_factory.create( deserializer.peek< the::ctci::Id >() ) );
  if ( !entity )
  {
    return entity;
  }
  entity->deserialize( deserializer );
  return entity;
}

Entity::Pointer
EntityFactory::create( const Data& data )
{
  Deserializer deserializer( data );
  return create( deserializer );
}


bool
EntityFactory::is_registered( the::ctci::Id id )
{
  return get().m_factory.is_registered( id );
}

}

