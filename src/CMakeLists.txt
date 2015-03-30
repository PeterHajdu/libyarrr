
set(SOURCE_FILES
  physical_parameters.cpp
  entity.cpp
  ship_control.cpp
  chat_message.cpp
  entity_factory.cpp
  delete_object.cpp
  object.cpp
  object_update.cpp
  object_behavior.cpp
  object_container.cpp
  object_exporter.cpp
  object_decorator.cpp
  object_identity.cpp
  basic_behaviors.cpp
  graphical_engine.cpp
  collider.cpp
  collision_checker.cpp
  destruction_handlers.cpp
  particle.cpp
  thruster.cpp
  clock_sync_catcher.cpp
  inventory.cpp
  canon.cpp
  object_creator.cpp
  item.cpp
  polygon.cpp
  shape.cpp
  coordinate_transformation.cpp
  shape_behavior.cpp
  resources.cpp
  command.cpp
  object_factory.cpp
  lua_engine.cpp
  lua_setup.cpp
  tile.cpp
  mission.cpp
  mission_factory.cpp
  character.cpp
  mission_container.cpp
  clock_exporter.cpp
  lua_agent.cpp
  crypto.cpp
  sha256.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION lib)

