
set(SOURCE_FILES
  object.cpp
  login.cpp
  event.cpp
  object_state_update.cpp
  command.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION lib)

