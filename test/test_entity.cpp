#include "test_entity.hpp"
#include <yarrr/entity.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(an_entity)
{
  It( has_polymorphic_ctci )
  {
    AssertThat( test_entity.ctci, Equals( base_referenced_test_entity.polymorphic_ctci() ) );
  }

  test::Entity test_entity;
  yarrr::Entity& base_referenced_test_entity{ test_entity };
};

