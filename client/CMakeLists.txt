cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_COMPILER "J:/newGame/MinGw/bin/gcc.exe")
project(guji C)
find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
find_program(GLSLC_PROGRAM glslc REQUIRED)
aux_source_directory(src SRC)
add_library(guji SHARED ${SRC})
target_include_directories(guji PUBLIC include)
target_compile_features(guji PUBLIC c_std_99)
target_link_libraries(guji PUBLIC Vulkan::Vulkan SDL2::SDL2)

#####################
# test
#####################
enable_testing()
add_subdirectory(test)