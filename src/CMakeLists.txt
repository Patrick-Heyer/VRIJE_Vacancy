PROJECT(Gesture_Therapy_Linux  C CXX )
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#C++11 features
SET(CMAKE_CXX_FLAGS "-std=c++0x")

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib )

INCLUDE_DIRECTORIES(./include/)

FIND_PACKAGE( Threads REQUIRED)

#INCLUDE_DIRECTORIES(../dependencies/PThreads/include)

ADD_SUBDIRECTORY(Plugin_API)
ADD_SUBDIRECTORY(Shared_Memory)
ADD_SUBDIRECTORY(Logger_API)

FILE(WRITE "../conf/plugins" "")

SET(OPENGL_GUI ON CACHE BOOL "description")
SET(PLUGINS_BTGRIPPER_READER OFF CACHE BOOL "description")
SET(PLUGINS_CAMERA_READER ON CACHE BOOL "description")
SET(PLUGINS_COLOR_TRACKER ON CACHE BOOL "description")
SET(PLUGINS_FILE_WRITER ON CACHE BOOL "description")
SET(PLUGINS_FUNCTION_TEST ON CACHE BOOL "description")
SET(PLUGINS_FUNCTION_CALLER ON CACHE BOOL "description")
SET(PLUGINS_JOYSTICK_READER ON CACHE BOOL "description")
SET(PLUGINS_JOYSTICK_WRITER ON CACHE BOOL "description")
SET(PLUGINS_NETWORK_CLIENT ON CACHE BOOL "description")
SET(PLUGINS_NETWORK_SERVER ON CACHE BOOL "description")
SET(PLUGINS_TUIO_CLIENT ON CACHE BOOL "description")
SET(PLUGINS_SCREEN_READER ON CACHE BOOL "description")

IF(OPENGL_GUI)
ADD_SUBDIRECTORY(Gui)
add_definitions(-DOPENGL_GUI)
ENDIF()

IF(PLUGINS_CAMERA_READER)
ADD_SUBDIRECTORY(Camera_Reader)
FILE(APPEND "../conf/plugins" "../lib/libCamera_Reader.so \n")
ENDIF()

IF(PLUGINS_COLOR_TRACKER)
ADD_SUBDIRECTORY(Color_Tracker)
FILE(APPEND "../conf/plugins" "../lib/libColor_Tracker.so \n")
ENDIF()

IF(PLUGINS_FILE_WRITER)
ADD_SUBDIRECTORY(File_Writer)
FILE(APPEND "../conf/plugins" "../lib/libFile_Writer.so \n")
ENDIF()

IF(PLUGINS_FUNCTION_CALLER)
ADD_SUBDIRECTORY(Function_Caller)
FILE(APPEND "../conf/plugins" "../lib/libFunction_Caller.so \n")
ENDIF()

IF(PLUGINS_FUNCTION_TEST)
ADD_SUBDIRECTORY(Function_Test)
FILE(APPEND "../conf/plugins" "../lib/libFunction_Test.so \n")
ENDIF()

IF(PLUGINS_JOYSTICK_READER)
ADD_SUBDIRECTORY(Joystick_Reader)
FILE(APPEND "../conf/plugins" "../lib/libJoystick_Reader.so \n")
ENDIF()

IF(PLUGINS_JOYSTICK_WRITER)
ADD_SUBDIRECTORY(Joystick_Writer)
FILE(APPEND "../conf/plugins" "../lib/libJoystick_Writer.so \n")
ENDIF()

IF(PLUGINS_NETWORK_CLIENT)
ADD_SUBDIRECTORY(Network_Client)
FILE(APPEND "../conf/plugins" "../lib/libNetwork_Client.so \n")
ENDIF()

IF(PLUGINS_NETWORK_SERVER)
ADD_SUBDIRECTORY(Network_Server)
FILE(APPEND "../conf/plugins" "../lib/libNetwork_Server.so \n")
ENDIF()

IF(PLUGINS_TUIO_CLIENT)
ADD_SUBDIRECTORY(Tuio_Client)
FILE(APPEND "../conf/plugins" "../lib/libTuio_Client.so \n")
ENDIF()

IF(PLUGINS_SCREEN_READER)
ADD_SUBDIRECTORY(Screen_Reader)
FILE(APPEND "../conf/plugins" "../lib/libScreen_Reader.so \n")
ENDIF()

ADD_EXECUTABLE(Gesture_Therapy Main.cpp)
TARGET_LINK_LIBRARIES(Gesture_Therapy Plugin_API Logger_API Shared_Memory -ldl)

if(OPENGL_GUI)
    TARGET_LINK_LIBRARIES(Gesture_Therapy OpenGL_GUI )
endif()

