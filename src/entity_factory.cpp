#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/log.hpp>
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
  try
  {
    EntityFactory& factory( get() );
    Entity::Pointer entity( factory.m_factory.create( deserializer.peek< the::ctci::Id >() ) );
    if ( !entity )
    {
      return nullptr;
    }

    entity->deserialize( deserializer );
    return entity;
  }
  catch ( std::runtime_error& error )
  {
    thelog( log::error )( error.what() );
  }

  return nullptr;
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

