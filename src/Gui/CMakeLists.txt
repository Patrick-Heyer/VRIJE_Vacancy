CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(../Plugin_API/)
INCLUDE_DIRECTORIES(../Shared_Memory/)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

if (WIN32 )
message("Building for Windows")

elseif(UNIX)
message("Building for Unix/Linux")

include_directories( ${OpenCV_INCLUDE_DIRS} )

ADD_LIBRARY(OpenGL_GUI SHARED button.cpp video.cpp widget.cpp GL_Window.cpp input_singleton.cpp textbox.cpp tab.cpp gui.cpp view2d.cpp console.cpp)
TARGET_LINK_LIBRARIES(OpenGL_GUI ${OpenCV_LIBS} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} Xmu Xext X11 m)
endif()
