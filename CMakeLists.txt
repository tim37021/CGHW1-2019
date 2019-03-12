cmake_minimum_required(VERSION 2.8)

project(app)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER inc/*.h src/*.h)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

add_executable(app ${SOURCE} ${HEADER})
target_include_directories(app PUBLIC inc)
target_link_libraries(app ${CONAN_LIBS})