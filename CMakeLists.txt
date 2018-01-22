#TODO: This entire file

cmake_minimum_required(VERSION 2.8)
project(Kondion)
set (Kondion_VERSION_MAJOR 2)
set (Kondion_VERSION_MINOR 0)
set (CMAKE_CXX_STANDARD 11)

configure_file (
  "src/config.h.in" 
  "src/config.h"
  )
#add_subdirectory(src)
file(GLOB_RECURSE KONDION_SOURCES "src/*.cpp")

add_executable(Kondion ${KONDION_SOURCES})
set_target_properties(Kondion PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

# Chrome V8

if(DEFINED ENV{V8_DIR})
  set (V8_DIRECTORY ENV{V8_DIR})
else()
  set (V8_DIRECTORY "/media/neal/EXEC/Depot/v8/v8")
endif()
include_directories(${V8_DIRECTORY})
file(GLOB_RECURSE V8_LIBRARIES "${V8_DIRECTORY}/out/native/obj.target/tools/gyp/*.a")
file(GLOB_RECURSE V8_THIRDPARTY_LIBRARIES "${V8_DIRECTORY}/out/native/obj.target/third_party/icu/*.a")
list(REMOVE_ITEM V8_LIBRARIES "${V8_DIRECTORY}/out/native/obj.target/tools/gyp/libv8_nosnapshot.a")
# GLFW3

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/glfw3)
target_link_libraries(Kondion glfw)

find_package(OpenGL REQUIRED)
target_include_directories(Kondion PUBLIC "${OPENGL_INCLUDE_DIR}")

# GLEW

find_package(GLEW REQUIRED)
target_include_directories(Kondion PUBLIC ${GLEW_INCLUDE_DIRS})

# Other things

include_directories(ThirdParty/stb)
include_directories(ThirdParty/glm)
include_directories(ThirdParty/tinydir)

message(STATUS "eeeee: " ${V8_LIBRARIES})
target_link_libraries(Kondion glfw ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} -Wl,--start-group ${V8_LIBRARIES} ${V8_THIRDPARTY_LIBRARIES} -Wl,--end-group ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt)


