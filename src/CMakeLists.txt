
set(SOURCE_FILES
  physical_parameters.cpp
  login.cpp
  event.cpp
  object_state_update.cpp
  command.cpp
  ship_control.cpp
  event_factory.cpp
  delete_object.cpp
  object.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION lib)

