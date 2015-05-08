#include <yarrr/loot.hpp>
#include <yarrr/cargo.hpp>
#include <yarrr/goods.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/object_destroyed.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/object_creator.hpp>
#include <yarrr/object_created.hpp>
#include <thectci/service_registry.hpp>

namespace yarrr
{

LootDropper::LootDropper()
  : ObjectBehavior( do_not_synchronize() )
  , m_owner_parameters( nullptr )
  , m_inventory( nullptr )
  , m_cargo( nullptr )
  , m_shape( nullptr )
{
}

void
LootDropper::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ObjectDestroyed >(
      [ this ]( const ObjectDestroyed& destroyed ){ handle_object_destroyed( destroyed ); } );
  m_owner_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
  m_inventory = &owner.components.component< Inventory >();
  m_cargo = &owner.components.component< CargoSpace >();
  m_shape = &owner.components.component< ShapeBehavior >().shape;
}

std::vector< Goods >
LootDropper::collect_goods() const
{
  std::vector< Goods > goods;

  for ( const auto& item : m_inventory->items() )
  {
    goods.push_back( Goods{ item.get().name(), "item", 1 } );
  }

  std::copy(
      std::begin( m_cargo->goods() ), std::end( m_cargo->goods() ),
      std::back_inserter( goods ) );

  return goods;
}

void
LootDropper::handle_object_destroyed( const ObjectDestroyed& ) const
{
  auto goods( collect_goods() );

  const auto& tiles( m_shape->tiles() );
  const size_t number_of_tiles( tiles.size() );

  for ( auto i( 0u ); i < number_of_tiles; ++i )
  {
    const bool is_last( i == ( number_of_tiles - 1 ) );

    std::vector< Goods >  slice_of_goods;
    if ( is_last )
    {
      slice_of_goods.swap( goods );
    }
    else if( !goods.empty() )
    {
      slice_of_goods.push_back( goods.back() );
      goods.pop_back();
    }

    const auto& tile( tiles[ i ] );
    the::ctci::service< yarrr::EngineDispatcher >().dispatch(
        ObjectCreated( create_loot( *m_owner_parameters, tile, slice_of_goods ) ) );
  }

}

ObjectBehavior::Pointer
LootDropper::clone() const
{
  return Pointer( new LootDropper() );
}

LootAttacher::LootAttacher()
  : ObjectBehavior( do_not_synchronize() )
  , m_cargo( nullptr )
{
}

void
LootAttacher::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::Collide >(
      [ this ]( const Collide& collide ){ handle_object_collided( collide ); } );
  m_cargo = &owner.components.component< CargoSpace >();
}

void
LootAttacher::handle_object_collided( const Collide& collide ) const
{
  if ( !collide.with.components.has_component< CargoSpace >() )
  {
    return;
  }

  add_goods_to( collide.with );
}

void
LootAttacher::add_goods_to( Object& new_owner ) const
{
  auto& new_owner_cargo( component_of< CargoSpace >( new_owner ) );
  for ( const auto& goods : m_cargo->goods() )
  {
    new_owner_cargo.add_goods( goods );
  }

  m_object->dispatcher.dispatch( ObjectDestroyed() );
}

ObjectBehavior::Pointer
LootAttacher::clone() const
{
  return Pointer( new LootAttacher() );
}
}

