cmake_minimum_required(VERSION 3.15)
project(guji C)
find_package(Vulkan REQUIRED)
find_program(GLSLC_PROGRAM glslc REQUIRED)
aux_source_directory(src SRC)
add_library(guji SHARED ${SRC})
message(STATUS "**${SDL2_INCLUDE_DIR}")
target_include_directories(guji PUBLIC include J:/newGame/VulkanSDK/1.3.211.0/Include/SDL2)
target_compile_features(guji PUBLIC c_std_99)
target_link_libraries(guji PUBLIC Vulkan::Vulkan J:/newGame/VulkanSDK/1.3.211.0/Lib/SDL2.lib J:/newGame/VulkanSDK/1.3.211.0/Lib/SDL2-static.lib J:/newGame/VulkanSDK/1.3.211.0/Lib/SDL2main.lib)

#####################
# test
#####################
enable_testing()
add_subdirectory(test)