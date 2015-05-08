#include <yarrr/inventory.hpp>
#include <yarrr/loot.hpp>
#include <yarrr/goods.hpp>
#include <yarrr/object_destroyed.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/object_created.hpp>
#include <yarrr/object_creator.hpp>
#include <yarrr/cargo.hpp>

#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

#include "test_synchronizable_behavior.hpp"
#include "test_services.hpp"
#include "test_item.hpp"

using namespace igloo;

Describe( a_loot_dropper )
{
  void add_cargo_to( yarrr::Object& object )
  {
    auto cargo( std::make_unique< yarrr::CargoSpace >() );
    for ( auto i( 0u ); i < number_of_goods; ++i )
    {
      cargo->add_goods( goods );
    }
    object.add_behavior( std::move( cargo ) );
  }

  void add_inventory_to( yarrr::Object& object )
  {
    object.add_behavior( std::make_unique< yarrr::Inventory >() );
    for ( auto i( 0u ); i < number_of_items; ++i )
    {
      object.add_behavior( std::make_unique< test::Item >() );
    }
  }

  void add_shape_to( yarrr::Object& object )
  {
    auto shape( std::make_unique< yarrr::ShapeBehavior >() );
    for ( auto i( 0u ); i < number_of_tiles; ++i )
    {
      shape->shape.add_tile( yarrr::Tile{ {0, 0}, {0, 0} } );
    }

    object.add_behavior( std::move( shape ) );
  }

  yarrr::Object::Pointer create_object()
  {
    auto object( std::make_unique< yarrr::Object >() );
    auto physical_behavior( std::make_unique< yarrr::PhysicalBehavior >() );
    physical_parameters = &physical_behavior->physical_parameters;
    physical_parameters->coordinate = far_from_the_origo;
    object->add_behavior( std::move( physical_behavior ) );
    add_shape_to( *object );
    add_inventory_to( *object );
    add_cargo_to( *object );

    object->add_behavior( std::make_unique< yarrr::LootDropper >() );
    return object;
  }

  void SetUp()
  {
    goods_in_loot.clear();
    object = create_object();
    created_objects.clear();

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::ObjectCreated >(
        [ this ]( const yarrr::ObjectCreated& object_created )
        {
          created_objects.emplace_back( std::move( object_created.object ) );
          auto& object( *created_objects.back() );

          created_objects_physical_parameters =
            &yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters;

          const auto& cargo( yarrr::component_of< yarrr::CargoSpace >( object ) );
          std::copy(
            std::begin( cargo.goods() ), std::end( cargo.goods() ),
            std::back_inserter( goods_in_loot ) );
        } );
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  bool objects_were_created()
  {
    return !created_objects.empty();
  }

  It( creates_objects_when_owner_is_destroyed )
  {
    AssertThat( objects_were_created(), Equals( true ) );
  }

  It( creates_an_object_for_each_tile_it_has )
  {
    AssertThat( created_objects.size(), Equals( number_of_tiles ) );
  }

  It( creates_objects_close_to_the_owner )
  {
    yarrr::Coordinate coordinate_difference(
        created_objects_physical_parameters->coordinate -
        physical_parameters->coordinate );
    yarrr::huplons_to_metres_in_place( coordinate_difference );
    AssertThat( yarrr::length_of( coordinate_difference ), IsLessThan( 30 ) );
  }

  It( creates_loot_objects_with_all_of_the_items_and_cargo )
  {
    AssertThat( goods_in_loot.size(), Equals( number_of_items + number_of_goods ) );
  }

  yarrr::Object::Pointer object;
  std::vector< yarrr::Object::Pointer > created_objects;
  yarrr::PhysicalParameters* created_objects_physical_parameters;
  yarrr::PhysicalParameters* physical_parameters;

  const yarrr::Goods goods{ "dog food", "food", 10 };

  const yarrr::Coordinate far_from_the_origo{ 100000, 1000000000 };
  const size_t number_of_items{ 3 };
  const size_t number_of_goods{ 7 };
  const size_t number_of_tiles{ 5 };

  std::vector< yarrr::Goods > goods_in_loot;
};


Describe( a_loot_attacher )
{
  void SetUp()
  {
    loot = yarrr::create_loot(
        yarrr::PhysicalParameters(),
        yarrr::Tile{ { 0, 0 }, { 10, 10 } },
        goods );
    object_with_cargo_space = test::create_ship();

    was_loot_destroyed = false;
    loot->dispatcher.register_listener< yarrr::ObjectDestroyed >(
        [ this ]( const yarrr::ObjectDestroyed& )
        {
          was_loot_destroyed = true;
        } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( registers_itsef_as_a_component )
  {
    AssertThat( loot->components.has_component< yarrr::LootAttacher >(), Equals( true ) );
  }

  It( adds_goods_to_the_colliding_objects_cargo_space )
  {
    loot->dispatcher.dispatch( yarrr::Collide( *object_with_cargo_space ) );
    const auto& cargo( yarrr::component_of< yarrr::CargoSpace >( *object_with_cargo_space ) );
    AssertThat( cargo.goods(), Equals( goods ) );
  }

  It( does_not_attach_cargo_to_objects_without_cargo_space )
  {
    yarrr::Object object_without_cargo_space;
    loot->dispatcher.dispatch( yarrr::Collide( object_without_cargo_space ) );
  }

  It( destroys_the_owner_if_it_collides )
  {
    loot->dispatcher.dispatch( yarrr::Collide( *object_with_cargo_space ) );
    AssertThat( was_loot_destroyed, Equals( true ) );
  }

  const std::vector< yarrr::Goods > goods{ yarrr::Goods{ "a", "b", 10 } };
  yarrr::Object::Pointer loot;
  yarrr::Object::Pointer object_with_cargo_space;

  bool was_loot_destroyed{ false };
};

