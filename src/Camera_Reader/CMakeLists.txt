PROJECT(Camera_Reader)
CMAKE_POLICY(SET CMP0015 NEW)

LINK_DIRECTORIES(../../dependencies/PThreads/lib/x64)

FIND_PACKAGE(OpenCV REQUIRED)
MESSAGE ( STATUS "OpenCV_LIBS = ${OpenCV_LIBS}" )
MESSAGE ( STATUS "OpenCV_VERSION= ${OpenCV_VERSION}")
MESSAGE ( STATUS "OpenCV_LIBS= ${OpenCV_LIBS}")

INCLUDE_DIRECTORIES(../Plugin_API/)
INCLUDE_DIRECTORIES(../Shared_Memory/)
INCLUDE_DIRECTORIES(../Logger_API/)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

ADD_LIBRARY(Camera_Reader MODULE  Main.cpp )
TARGET_LINK_LIBRARIES(Camera_Reader Plugin_API Shared_Memory Logger_API ${OpenCV_LIBS})
