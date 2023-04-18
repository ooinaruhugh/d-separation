cmake_minimum_required(VERSION 3.10)

project(d_separation)

include_directories(
    src
)

add_library(d_separation STATIC src/d_separation.cc)
target_compile_features(d_separation PRIVATE cxx_std_20)

link_libraries(d_separation)
add_executable(demo01 plain.cc)target_compile_features(demo01 PRIVATE cxx_std_20)