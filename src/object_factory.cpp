#include <yarrr/object_factory.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/log.hpp>
#include <yarrr/object_decorator.hpp>
#include <yarrr/object_created.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/destruction_handlers.hpp>

#include <thectci/service_registry.hpp>

namespace
{

void register_object_factory( const std::string& name, sol::function factory )
{
  the::ctci::service< yarrr::ObjectFactory >().register_creator(
      name,
      [ factory, name ]()
      {
        thelog( yarrr::log::info )( "Calling lua factory method:", name );
        try
        {
          auto new_object( yarrr::Object::create() );
          thelog( yarrr::log::debug )( "Sending object to lua factory method:", new_object.get(), new_object->id() );
          factory.call( yarrr::ObjectDecorator( *new_object ) );
          return std::move( new_object );
        }
        catch( std::exception& e )
        {
          thelog( yarrr::log::info )( "Lua factory method failed:", name, e.what() );
        }
        return yarrr::Object::Pointer( nullptr );
      } );
  thelog( yarrr::log::info )( "Registered lua factory method for object type:", name );
}

void create_object_callback( const std::string& key, sol::function decorator )
{
  the::ctci::service< yarrr::ObjectFactory >().create_object( key, decorator );
}

}

namespace yarrr
{

ObjectFactory::ObjectFactory()
  : m_factory_model( "object_factory", yarrr::LuaEngine::model() )
  , m_register_model( "register_factory", m_factory_model, &register_object_factory )
  , m_create_model( "create_object", m_factory_model, &create_object_callback )
{
}

yarrr::Object::Pointer
ObjectFactory::create_a( const std::string& key ) const
{
  Creators::const_iterator creator( m_creators.find( key ) );
  if ( creator == m_creators.end() )
  {
    thelog( yarrr::log::warning )( "Unknown object key requested: ", key );
    return nullptr;
  }

  auto new_object( (creator->second)() );

  if ( !new_object )
  {
    return nullptr;
  }

  new_object->add_behavior( delete_when_destroyed() );
  return std::move( new_object );
}

void
ObjectFactory::register_creator(
    const std::string& key,
    Creator creator )
{
  m_creators[ key ] = creator;
  m_object_types.push_back( key );
  thelog( yarrr::log::info )( "Registered creator for type: ", key );
}

const ObjectFactory::ObjectTypeList&
ObjectFactory::objects() const
{
  return m_object_types;
}


void
ObjectFactory::create_object( const std::string& key, sol::function decorator ) const
{
  auto new_object( create_a( key ) );

  if (!new_object)
  {
    thelog( log::error )( "Unable to create object with type:", key );
    return;
  }

  decorator( ObjectDecorator( *new_object ) );
  engine_dispatch( ObjectCreated( std::move( new_object ) ) );
}

}

