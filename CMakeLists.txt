cmake_minimum_required (VERSION 3.13)
if (DEFINED CMAKE_TOOLCHAIN_FILE)
    message (STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

project (othello)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    option (BUILD_SHARED_LIBS "Build shared libraries" ON)
else ()
    option (BUILD_SHARED_LIBS "Build shared libraries" OFF)
endif ()
message (STATUS "Building shared libs: ${BUILD_SHARED_LIBS}")
set (CMAKE_LINK_DEPENDS_NO_SHARED ON)

set (OpenGL_GL_PREFERENCE GLVND)
find_package (Threads REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)
find_package (Boost REQUIRED)

target_link_libraries (glfw INTERFACE GL)

add_subdirectory (vendor)
add_subdirectory (src)
