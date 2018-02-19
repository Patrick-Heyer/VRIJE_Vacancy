CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(../Plugin_API/)
INCLUDE_DIRECTORIES(../Shared_Memory/)
INCLUDE_DIRECTORIES(../Logger_API/)

if (WIN32 )
message("Building for Windows")
CMAKE_POLICY(SET CMP0015 NEW)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

INCLUDE_DIRECTORIES(../../dependencies/VJoy/inc/)

LINK_DIRECTORIES(../../dependencies/PThreads/lib)
LINK_DIRECTORIES(../../dependencies/VJoy/lib)


ADD_LIBRARY(Joystick_Writer SHARED Main.cpp dllmain.cpp )
TARGET_LINK_LIBRARIES(Joystick_Writer  Plugin_API Shared_Memory Logger_API pthreadVC1 vJoyInterface)

elseif(UNIX)
message("Building for Unix/Linux")
ADD_LIBRARY(Joystick_Writer SHARED Main.cpp )
TARGET_LINK_LIBRARIES(Joystick_Writer  Plugin_API Shared_Memory Logger_API ${CMAKE_THREAD_LIBS_INIT})
endif()






