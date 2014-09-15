#include "test_remote_object.hpp"
#include <yarrr/object.hpp>

namespace test
{

yarrr::Object::Pointer create_remote_object_from( yarrr::Object& original_object )
{
  auto object_update( original_object.generate_update() );
  const yarrr::Data serialized_update( object_update->serialize() );
  yarrr::ObjectUpdate deserialized_update;
  deserialized_update.deserialize( serialized_update );
  return deserialized_update.create_object();
}

}

