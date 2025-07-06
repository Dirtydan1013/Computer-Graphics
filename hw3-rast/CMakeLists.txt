cmake_minimum_required (VERSION 2.8)
project (cghw-rast)

#find_package(GLUT)
find_package(OpenGL)
find_package(glm)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

#if(CMAKE_COMPILER_IS_GNUCXX)
#    add_definitions(-std=gnu++11)
#endif()


# cghw-rast
add_executable(rast hw-rast.cpp swgl-rast.cpp)
target_link_libraries(rast ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES}  )