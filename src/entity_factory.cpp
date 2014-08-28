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
  the::ctci::Id class_id;
  try
  {
    class_id = deserializer.peek< the::ctci::Id >();
  }
  catch ( std::runtime_error& error )
  {
    thelog( log::error )( "While parsing class id: ", error.what() );
    return nullptr;
  }

  try
  {
    EntityFactory& factory( get() );
    Entity::Pointer entity( factory.m_factory.create( class_id ) );
    if ( !entity )
    {
      return nullptr;
    }

    entity->deserialize( deserializer );
    return entity;
  }
  catch ( std::runtime_error& error )
  {
    thelog( log::error )( error.what(), "class_id=", class_id );
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

