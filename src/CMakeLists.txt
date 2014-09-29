
set(SOURCE_FILES
  physical_parameters.cpp
  login.cpp
  entity.cpp
  command.cpp
  chat_message.cpp
  entity_factory.cpp
  delete_object.cpp
  object.cpp
  object_update.cpp
  object_behavior.cpp
  object_container.cpp
  basic_behaviors.cpp
  graphical_engine.cpp
  collider.cpp
  destruction_handlers.cpp
  particle.cpp
  thruster.cpp
  clock_sync_catcher.cpp
  inventory.cpp
  canon.cpp
  object_creator.cpp
  item.cpp
  shape.cpp
  shape_behavior.cpp
  resources.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION lib)

