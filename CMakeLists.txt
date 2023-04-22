cmake_minimum_required(VERSION 3.10)

project(d_separation)

include(CTest)

include_directories(
    src
)

add_library(d_separation STATIC src/d_separation.cc)
target_compile_features(d_separation PRIVATE cxx_std_20)

add_library(star_separation STATIC src/star_separation.cc)
target_compile_features(star_separation PRIVATE cxx_std_20)

add_executable(demo01 plain.cc)
target_link_libraries(demo01 d_separation)
target_compile_features(demo01 PRIVATE cxx_std_20)

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