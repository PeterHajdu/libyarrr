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
EntityFactory::create( const Data& data )
{
  if ( data.size() < sizeof( the::ctci::Id ) )
  {
    return nullptr;
  }

  EntityFactory& factory( get() );
  Entity::Pointer entity( factory.m_factory.create( extract< the::ctci::Id >( &data[0] ) ) );
  if ( !entity )
  {
    return entity;
  }
  entity->deserialize( data );
  return entity;
}


bool
EntityFactory::is_registered( the::ctci::Id id )
{
  return get().m_factory.is_registered( id );
}

}

