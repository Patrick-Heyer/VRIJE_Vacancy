CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(../Plugin_API/)
INCLUDE_DIRECTORIES(../Shared_Memory/)

if (WIN32 )
message("Building for Windows")
CMAKE_POLICY(SET CMP0015 NEW)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)


LINK_DIRECTORIES(../../dependencies/PThreads/lib)


ADD_LIBRARY(Joystick_Reader SHARED Main.cpp Joypad.cpp )
TARGET_LINK_LIBRARIES(Joystick_Reader  Plugin_API Shared_Memory pthreadVC1 vJoyInterface)

elseif(UNIX)
message("Building for Unix/Linux")
ADD_LIBRARY(Joystick_Reader SHARED Main.cpp Joypad.cpp )
TARGET_LINK_LIBRARIES(Joystick_Reader  Plugin_API Shared_Memory ${CMAKE_THREAD_LIBS_INIT})
endif()



