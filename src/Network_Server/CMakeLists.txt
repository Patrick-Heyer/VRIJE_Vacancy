CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(../Plugin_API/)
INCLUDE_DIRECTORIES(../Shared_Memory/)

if (WIN32 )
message("Building for Windows")
CMAKE_POLICY(SET CMP0015 NEW)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

LINK_DIRECTORIES(../../dependencies/PThreads/lib)

ADD_LIBRARY(Network_Server SHARED Main.cpp NetThread.cpp gsNetPoll_g++.cpp)
TARGET_LINK_LIBRARIES(Network_Server  Plugin_API Shared_Memory rt ${CMAKE_THREAD_LIBS_INIT} )


elseif(UNIX)
message("Building for Unix/Linux")
ADD_LIBRARY(Network_Server SHARED Main.cpp NetThread.cpp gsNetPoll_g++.cpp )
TARGET_LINK_LIBRARIES(Network_Server  Plugin_API Shared_Memory ${CMAKE_THREAD_LIBS_INIT})
endif()
