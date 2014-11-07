#include <yarrr/object_factory.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/log.hpp>
#include <thectci/service_registry.hpp>
#include <yarrr/object_creator.hpp>

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
          factory.call( *new_object );
          return std::move( new_object );
        }
        catch( std::exception& e )
        {
          thelog( yarrr::log::info )( "Lua factory method failed:", name, e.what() );
        }
        return yarrr::create_ship();
      } );
  thelog( yarrr::log::info )( "Registered lua factory method for object type:", name );
}

}

namespace yarrr
{

ObjectFactory::ObjectFactory()
  : m_factory_model( "object_factory", yarrr::LuaEngine::model() )
  , m_register_model( "register_factory", m_factory_model, &register_object_factory )
{
}

yarrr::Object::Pointer
ObjectFactory::create_a( const std::string& key )
{
  Creators::const_iterator creator( m_creators.find( key ) );
  if ( creator == m_creators.end() )
  {
    thelog( yarrr::log::warning )( "Unknown object key requested: ", key );
    return nullptr;
  }

  return m_creators[ key ]();
}

void
ObjectFactory::register_creator(
    const std::string& key,
    Creator creator )
{
  m_creators[ key ] = creator;
  thelog( yarrr::log::info )( "Registered creator for type: ", key );
}

}

