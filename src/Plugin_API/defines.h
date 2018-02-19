///
/// \file   defines.h
/// \author Patrick Heyer patrickhey@prodigy.net.mx
/// \brief  Win32 decorator macros for the Core and Plugin APIs
///

#ifndef DEFINES_H
#define DEFINES_H

#ifdef _WIN32
#ifdef BUILDING_CORE 
#define CORE_API   __declspec(dllexport)
#define PLUGIN_API __declspec(dllimport)
#else
#define CORE_API   __declspec(dllimport)
#define PLUGIN_API __declspec(dllexport)
#endif
#else
#define CORE_API
#define PLUGIN_API
#endif


#ifdef _WIN32
#pragma warning ( disable : 4251 )
#endif

#endif
