#include <yarrr/delete_object.hpp>
#include <thectci/id.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/physical_parameters.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(delete_object)
{
  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::DeleteObject::ctci ), Equals( true ) );
  }

  It( is_serializable_and_deserializable )
  {
    yarrr::DeleteObject delete_object( object_id );
    yarrr::Data serialized_delete_object( delete_object.serialize() );

    yarrr::DeleteObject deserialized_delete_object;
    deserialized_delete_object.deserialize( serialized_delete_object );
    AssertThat( deserialized_delete_object.object_id(), Equals( delete_object.object_id() ) );
  }

  const yarrr::PhysicalParameters::Id object_id{ 246 };
};

