#include "test_entity.hpp"
#include <yarrr/entity_factory.hpp>
#include <yarrr/entity.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(the_entity_factory)
{
  void SetUp()
  {
    if ( !yarrr::EntityFactory::is_registered( test::Entity::ctci ) )
    {
      yarrr::EntityFactory::register_class<test::Entity>();
    }
    serialized_entity = test::Entity( test_data ).serialize();
    entity = yarrr::EntityFactory::create( serialized_entity );
  }

  It( creates_instances_of_a_registered_class )
  {
    AssertThat( entity->polymorphic_ctci(), Equals( test::Entity::ctci ) );
  }

  It( deserializes_the_class )
  {
    const test::Entity& test_entity( static_cast<const test::Entity&>( *entity ) );
    AssertThat( test_entity.data, Equals( test_data ) );
  }

  It( it_returns_nullptr_if_the_class_is_not_registered )
  {
    const yarrr::Data null_data( 10, 0 );
    yarrr::Entity::Pointer entity_pointer( yarrr::EntityFactory::create( null_data ) );
    AssertThat( bool( entity_pointer ), Equals( false ) );
  }

  It( it_returns_nullptr_if_message_is_smaller_than_class_id )
  {
    const yarrr::Data small_data( 0, 0 );
    yarrr::Entity::Pointer entity_pointer( yarrr::EntityFactory::create( small_data ) );
    AssertThat( bool( entity_pointer ), Equals( false ) );
  }

  It( knows_if_a_class_is_registered )
  {
    AssertThat( yarrr::EntityFactory::is_registered( test::Entity::ctci ), Equals( true ) );
    AssertThat( yarrr::EntityFactory::is_registered( 0 ), Equals( false ) );
  }

  int test_data{ 13 };
  yarrr::Data serialized_entity;
  yarrr::Entity::Pointer entity;
};

namespace
{

class AutoTestEntity : public yarrr::Entity
{
  public:
    add_polymorphic_ctci( "auto_test_entity" );
};

}

Describe(an_auto_register)
{
  void SetUp()
  {
  }

  It( registers_entity_class_to_entity_factory )
  {
    yarrr::AutoEntityRegister< AutoTestEntity >();
    AssertThat( yarrr::EntityFactory::is_registered( AutoTestEntity::ctci ), Equals( true ) );
  }
};

