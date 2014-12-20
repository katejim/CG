cmake_minimum_required(VERSION 2.8.4)
project(task2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)

set(SOURCE_FILES
    main.cpp
    shader.cpp
    task2.cpp
    objloader.cpp
    tangentspace.cpp
    controls.cpp
        )

add_executable(task2 ${SOURCE_FILES})

set(LIBS AntTweakBar glfw3 GLEW)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LIBS ${LIBS} Xext X11 Xrandr Xi Xxf86vm pthread)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

target_link_libraries(task2 ${OPENGL_LIBRARY} ${LIBS})
