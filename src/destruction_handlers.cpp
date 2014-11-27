#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/log.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/object_decorator.hpp>
#include <thectci/service_registry.hpp>

namespace yarrr
{
CallWhenDestroyed::CallWhenDestroyed( const LuaFunction& lua_function )
  : ObjectBehavior( do_not_synchronize() )
  , m_callback(
      [ lua_function ]( Object& object )
      {
        lua_function.call( yarrr::ObjectDecorator( object ) );
      } )
{
}

CallWhenDestroyed::CallWhenDestroyed( Callback callback )
  : ObjectBehavior( do_not_synchronize() )
  , m_callback( callback )
{
}

void
CallWhenDestroyed::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >(
        [ this ]( const ObjectDestroyed& destroyed ){ handle_object_destroyed( destroyed ); } );
}

ObjectBehavior::Pointer
CallWhenDestroyed::clone() const
{
  return Pointer( new CallWhenDestroyed( m_callback ) );
}


void
CallWhenDestroyed::handle_object_destroyed( const ObjectDestroyed& ) const
{
  thelog_trace( yarrr::log::debug, "Call when destroyed." );
  m_callback( *m_object );
}

ObjectBehavior::Pointer
delete_when_destroyed()
{
  return std::make_unique< CallWhenDestroyed >(
      []( Object& object )
      {
        thelog( yarrr::log::debug )( "Delete when destroyed deletes object", object.id() );
        the::ctci::service< EngineDispatcher >().dispatch( DeleteObject( object.id() ) );
      } );
}


ObjectBehavior::Pointer
kill_player_when_destroyed()
{
  return std::make_unique< CallWhenDestroyed >(
      []( Object& object )
      {
        thelog( yarrr::log::debug )( "Kill player when destroyed.", object.id() );
        the::ctci::service< EngineDispatcher >().dispatch( PlayerKilled( object.id() ) );
      } );
}


PlayerKilled::PlayerKilled( const Object::Id& id )
  : object_id( id )
{
}


}

