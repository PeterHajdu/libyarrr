#include <yarrr/entity.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestEntity : public yarrr::Entity
  {
    public:
      add_polymorphic_ctci( "test_entity" );
  };
}

Describe(an_entity)
{
  It( has_polymorphic_ctci )
  {
    AssertThat( test_entity.ctci, Equals( base_referenced_test_entity.polymorphic_ctci() ) );
  }

  TestEntity test_entity;
  yarrr::Entity& base_referenced_test_entity{ test_entity };
};

