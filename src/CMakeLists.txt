
set(SOURCE_FILES
  physical_parameters.cpp
  login.cpp
  entity.cpp
  command.cpp
  chat_message.cpp
  ship_control.cpp
  entity_factory.cpp
  delete_object.cpp
  object.cpp
  object_container.cpp
  basic_behaviors.cpp
  stream_to_chat.cpp
  graphical_engine.cpp
  cli.cpp
  terminal.cpp
  collider.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION lib)

