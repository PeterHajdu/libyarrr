#pragma once

#include <yarrr/object.hpp>
#include <yarrr/entity_factory.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

template < typename Behavior >
void assert_that_it_is_a_synchronizable_behavior()
{
  Behavior behavior;
  AssertThat( behavior.should_synchronize == yarrr::ObjectBehavior::synchronize, Equals( true ) );
  AssertThat( behavior.clone()->id(), Equals( behavior.id() ) );
  AssertThat( yarrr::EntityFactory::is_registered( Behavior::ctci ), Equals( true ) );
}

}

