#include <yarrr/object_identity.hpp>
#include "test_synchronizable_behavior.hpp"
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_object_identity )
{

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( std::make_unique< yarrr::ObjectIdentity >( captain ) );
    identity = &yarrr::component_of< yarrr::ObjectIdentity >( *object );
  }

  It( can_be_synchronized )
  {
    test::assert_that_it_is_a_synchronizable_behavior< yarrr::ObjectIdentity >();
  }

  It( stores_captains_name )
  {
    AssertThat( identity->captain(), Equals( captain ) );
  }

  It( can_be_serialized_and_deserialized )
  {
    const auto serialized_data( identity->serialize() );
    yarrr::Entity::Pointer entity( yarrr::EntityFactory::create( serialized_data ) );
    const yarrr::ObjectIdentity& deserialized_identity(
        static_cast< const yarrr::ObjectIdentity& >( *entity ) );
    AssertThat( deserialized_identity.captain(), Equals( captain ) );
  }

  It( can_be_cloned )
  {
    yarrr::ObjectBehavior::Pointer clone{ identity->clone() };
    const yarrr::ObjectIdentity& identity_clone{ static_cast< const yarrr::ObjectIdentity& >( *clone ) };
    AssertThat( identity_clone.captain(), Equals( captain ) );
  }

  yarrr::Object::Pointer object;
  yarrr::ObjectIdentity* identity;
  const std::string captain{ "Black Beard"};
};

