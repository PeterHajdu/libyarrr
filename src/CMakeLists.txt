
set(SOURCE_FILES
  physical_parameters.cpp
  login.cpp
  event.cpp
  object_state_update.cpp
  command.cpp
  chat_message.cpp
  ship_control.cpp
  event_factory.cpp
  delete_object.cpp
  object.cpp
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
target_link_libraries(yarrrgraphics theconf ${LIBS})
install(TARGETS yarrrgraphics DESTINATION lib)

