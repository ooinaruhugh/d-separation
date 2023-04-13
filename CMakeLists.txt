cmake_minimum_required(VERSION 3.10)

project(d_separation)

add_executable(demo01 plain.cc)
target_compile_features(demo01 PRIVATE cxx_std_20)