
# required for igloo:
add_definitions(-Wno-deprecated-declarations)
include_directories(igloo)

set(TEST_SOURCE_FILES
    test_runner.cpp
    test_physical_parameters.cpp
    test_vector.cpp
    test_clock_synchronizer.cpp
    test_bitmagic.cpp
    test_login.cpp
    test_event.cpp
    test_object_state_update.cpp
    test_command.cpp
    test_chat_message.cpp
    test_ship_control.cpp
    test_event_factory.cpp
    test_callback.cpp
    test_types.cpp
    test_delete_object.cpp
    test_object.cpp
    test_connection_wrapper.cpp
    test_basic_behaviors.cpp
    test_graphical_engine.cpp
    test_cli.cpp
    test_terminal.cpp
    test_stream_to_chat.cpp
    )


add_executable(test_runner EXCLUDE_FROM_ALL ${TEST_SOURCE_FILES})
target_link_libraries(test_runner yarrr yarrrgraphics theconf ${LIBS})

get_target_property(TEST_RUNNER_BIN test_runner LOCATION) # for running

add_custom_target(test DEPENDS test_runner)
add_custom_command(TARGET test COMMAND valgrind --leak-check=full --show-reachable=yes --log-file="valgrind.log" ${TEST_RUNNER_BIN} --verbose DEPENDS test_runner)
