cmake_minimum_required (VERSION 2.8)

project(HelloOpenGL)

include_directories(${CMAKE_SOURCE_DIR}/src/)

include_directories(${CMAKE_SOURCE_DIR}/common/)

include_directories(${CMAKE_SOURCE_DIR}/external/include)

include_directories(SYSTEM external/glm)

link_directories(${CMAKE_SOURCE_DIR}/external/lib/)

set(LIBS glew)
list(APPEND LIBS glfw)
list(APPEND LIBS soil)
list(APPEND LIBS assimp)
find_package(OpenGL REQUIRED)
list(APPEND LIBS ${OPENGL_gl_LIBRARY}) # setting LIBS for the first time
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
list(APPEND LIBS ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xinerama_LIB} ${X11_Xi_LIB} ${X11_Xcursor_LIB})
find_package(Threads REQUIRED)
list(APPEND LIBS ${CMAKE_THREAD_LIBS_INIT})

INCLUDE_DIRECTORIES(/System/Library/Frameworks)
FIND_LIBRARY(Cocoa_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED(Cocoa_LIBRARY OpenGL_LIBRARY)
SET(APPLE_LIBS ${Cocoa_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
set(LIBS ${LIBS} ${APPLE_LIBS})

# clang && macosx
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
# c++11
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
# clang && debug adds address sanitizer
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND NOT APPLE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()

set(TargetName HelloGL)
file(GLOB SOURCE "${CMAKE_SOURCE_DIR}/src/*.h" "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(${TargetName} ${SOURCE})
target_link_libraries(${TargetName} ${LIBS})
set_target_properties(${TargetName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/")

set(RES ${CMAKE_SOURCE_DIR}/res)
file(COPY ${RES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)     
