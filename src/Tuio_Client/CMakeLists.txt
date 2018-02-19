CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(../Plugin_API/)
INCLUDE_DIRECTORIES(../Shared_Memory/)
LINK_DIRECTORIES(../../dependencies)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/../lib)

if (WIN32 )
message("Building for Windows")
CMAKE_POLICY(SET CMP0015 NEW)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)


LINK_DIRECTORIES(../../dependencies/PThreads/lib)


ADD_LIBRARY(Tuio_Client SHARED Main.cpp dllmain.cpp )
TARGET_LINK_LIBRARIES(Tuio_Client  Plugin_API Shared_Memory pthreadVC1 vJoyInterface)

elseif(UNIX)
message("Building for Unix/Linux")

include_directories(osc/)
include_directories(ip/)

ADD_LIBRARY(Tuio_Client SHARED Main.cpp )
TARGET_LINK_LIBRARIES(Tuio_Client Plugin_API Shared_Memory TUIO ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
if(OPENGL_GUI)
TARGET_LINK_LIBRARIES(Tuio_Client OpenGL_GUI)
endif()
endif()



