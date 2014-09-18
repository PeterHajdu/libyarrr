#include "test_remote_object.hpp"
#include <yarrr/object.hpp>
#include <yarrr/object_update.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/service_registry.hpp>

namespace test
{

yarrr::Object::Pointer create_remote_object_from( yarrr::Object& original_object )
{
  auto object_update( original_object.generate_update() );
  const yarrr::Data serialized_update( object_update->serialize() );
  yarrr::Entity::Pointer deserialized_entity( yarrr::EntityFactory::create( serialized_update ) );
  yarrr::ObjectUpdate& deserialized_update( static_cast< yarrr::ObjectUpdate& >( *deserialized_entity ) );
  return deserialized_update.create_object();
}

}

