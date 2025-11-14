# All test source files (no need to list them manually)
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    set(target_name "unit_${test_name}")

    add_executable(${target_name} ${test_src})

    target_link_libraries(${target_name}
        PRIVATE
        GTest::gtest_main
        CompilationEngine
        Tokenizer
        VMWriter
    )

    include(GoogleTest)
    gtest_discover_tests(${target_name})
endforeach()
