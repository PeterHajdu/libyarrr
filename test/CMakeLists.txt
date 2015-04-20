
# required for igloo:
add_definitions(-Wno-deprecated-declarations)

set(TEST_SOURCE_FILES
    test_runner.cpp
    test_physical_parameters.cpp
    test_vector.cpp
    test_clock_synchronizer.cpp
    test_bitmagic.cpp
    test_entity.cpp
    test_ship_control.cpp
    test_command.cpp
    test_chat_message.cpp
    test_entity_factory.cpp
    test_callback.cpp
    test_types.cpp
    test_delete_object.cpp
    test_object.cpp
    test_object_decorator.cpp
    test_object_update.cpp
    test_object_behavior.cpp
    test_connection_wrapper.cpp
    test_basic_behaviors.cpp
    test_thruster.cpp
    test_graphical_engine.cpp
    test_object_container.cpp
    test_engine_dispatcher.cpp
    test_main_thread_callback_queue.cpp
    test_services.cpp
    test_collider.cpp
    test_collision_checker.cpp
    test_destruction_handler.cpp
    test_particles.cpp
    test_damage_causer.cpp
    test_inventory.cpp
    test_canon.cpp
    test_object_creator.cpp
    test_item.cpp
    test_tile.cpp
    test_polygon.cpp
    test_coordinate_transformation.cpp
    test_shape.cpp
    test_shape_behavior.cpp
    test_remote_object.cpp
    test_resources.cpp
    test_lua_engine.cpp
    test_lua_setup.cpp
    test_object_factory.cpp
    test_mission.cpp
    test_mission_factory.cpp
    test_object_exporter.cpp
    test_character.cpp
    test_mission_exporter.cpp
    test_mission_container.cpp
    test_clock_exporter.cpp
    test_object_identity.cpp
    test_lua_agent.cpp
    test_crypto.cpp
    test_id_generator.cpp
    )


add_executable(test_runner EXCLUDE_FROM_ALL ${TEST_SOURCE_FILES})


target_link_libraries(test_runner yarrr thelog theconf thectci themodel thetime lua ${LIBS})

get_target_property(TEST_RUNNER_BIN test_runner LOCATION) # for running

find_program( VALGRIND_PATH valgrind )

if(VALGRIND_PATH)
  set(RUNNER
    valgrind --leak-check=full --show-reachable=yes --log-file="valgrind.log"
    )
endif()

add_custom_target(test DEPENDS test_runner)
add_custom_command(TARGET test COMMAND ${RUNNER} ${TEST_RUNNER_BIN} --verbose DEPENDS test_runner)
