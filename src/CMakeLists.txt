
set(SOURCE_FILES
  physical_parameters.cpp
  login.cpp
  entity.cpp
  object_state_update.cpp
  command.cpp
  chat_message.cpp
  ship_control.cpp
  entity_factory.cpp
  delete_object.cpp
  object.cpp
  object_container.cpp
  basic_behaviors.cpp
  stream_to_chat.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION lib)

set(GRAPHIC_SOURCE_FILES
  graphical_engine.cpp
  cli.cpp
  terminal.cpp
  )

add_library(yarrrgraphics STATIC ${GRAPHIC_SOURCE_FILES})
target_link_libraries(yarrrgraphics ${LIBS})
install(TARGETS yarrrgraphics DESTINATION lib)

