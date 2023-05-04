cmake_minimum_required(VERSION 3.10)

project(d_separation)

include(CTest)

include_directories(
    src
)

if (DEFINED EMSCRIPTEN)
    add_executable(markov 
        src/wasm.cc src/d_separation.cc src/star_separation.cc
    )

    target_compile_features(markov PRIVATE cxx_std_20)

    set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
    
    target_link_libraries(markov embind)

	set_target_properties(markov PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=0")
	set_target_properties(markov PROPERTIES LINK_FLAGS    "-Os -s WASM=1 -s SIDE_MODULE=0 -s STANDALONE_WASM -s ENVIRONMENT=web -s MODULARIZE=1 -s EXPORT_ES6=1 --bind --no-entry")
else()
    add_library(d_separation STATIC src/d_separation.cc)
    target_compile_features(d_separation PRIVATE cxx_std_20)

    add_library(star_separation STATIC src/star_separation.cc)
    target_compile_features(star_separation PRIVATE cxx_std_20)

    add_executable(demo01 plain.cc)
    target_link_libraries(demo01 d_separation)
    target_link_libraries(demo01 star_separation)
    target_compile_features(demo01 PRIVATE cxx_std_20)

    add_executable(demo_enumerate demo_enumerate.cc)
    target_link_libraries(demo_enumerate d_separation)
    target_link_libraries(demo_enumerate star_separation)
    target_compile_features(demo_enumerate PRIVATE cxx_std_20)

    add_executable(demo_star demo_star.cc)
    target_link_libraries(demo_star star_separation)
    target_compile_features(demo_star PRIVATE cxx_std_20)

    # Testing

    add_executable(test_cassiopeia tests/test_driver.cc tests/cassiopeia.cc)
    target_link_libraries(test_cassiopeia d_separation)
    target_compile_features(test_cassiopeia PRIVATE cxx_std_20)

    add_executable(test_diamond tests/test_driver.cc tests/diamond.cc)
    target_link_libraries(test_diamond d_separation)
    target_compile_features(test_diamond PRIVATE cxx_std_20)

    add_test(NAME GlobalMarkovForCassiopeia COMMAND test_cassiopeia)
    add_test(NAME GlobalMarkovForDiamond COMMAND test_diamond)

    enable_testing()
endif()