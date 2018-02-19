PROJECT(Gesture_Therapy)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


INCLUDE_DIRECTORIES(../../dependencies/libpq/include)



ADD_LIBRARY(Plugin_API SHARED coreapi.cpp pluginapi.cpp pluginmanager.cpp ../include/ConfigFile.cpp)
SET_TARGET_PROPERTIES(Plugin_API  PROPERTIES DEFINE_SYMBOL BUILDING_CORE )
