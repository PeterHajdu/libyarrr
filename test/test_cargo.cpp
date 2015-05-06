#include <yarrr/cargo.hpp>
#include <yarrr/goods.hpp>
#include <yarrr/object_destroyed.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/destruction_handlers.hpp>

#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

#include "test_synchronizable_behavior.hpp"
#include "test_remote_object.hpp"

using namespace igloo;

Describe( a_cargo_space )
{
  void SetUp()
  {
    cargo_space = std::make_unique< yarrr::CargoSpace >();
    cargo_space->add_goods( goods );
  }

  It( is_synchronizable )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::CargoSpace >();
  }

  It( keeps_a_list_of_goods )
  {
    AssertThat( cargo_space->goods(), Contains( goods ) );
  }

  It( serializes_and_deserializes_goods )
  {
    yarrr::Object an_object;
    an_object.add_behavior( std::move( cargo_space ) );
    auto remote_object( test::create_remote_object_from( an_object ) );
    const auto& remote_goods( yarrr::component_of< yarrr::CargoSpace >( *remote_object ).goods() );
    AssertThat( remote_goods, Contains( goods ) );
  }

  It( should_force_synchronization_if_goods_are_added )
  {
    AssertThat( cargo_space->should_synchronize(), Equals( true ) );
  }

  std::unique_ptr< yarrr::CargoSpace > cargo_space;
  const yarrr::Goods goods{ "canon", "item", 1 };
};

