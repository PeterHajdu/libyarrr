
set(SOURCE_FILES
  socket_pool.cpp
  ship.cpp
    )

add_library(yarrr STATIC ${SOURCE_FILES})
target_link_libraries(yarrr ${LIBS})
install(TARGETS yarrr DESTINATION ${deploy_directory}/lib)

