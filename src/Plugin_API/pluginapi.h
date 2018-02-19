///
/// \file   pluginapi.h
/// \author Patrick Heyer patrickhey@prodigy.net.mx
/// \brief  An API that lets users write plugins.
///


#ifndef PLUGINAPI_H
#define PLUGINAPI_H

#include "defines.h"
#include "plugin.h"

#define PLUGIN_API_VERSION  1

#define CORE_FUNC     extern "C" CORE_API
#define PLUGIN_FUNC   extern "C" PLUGIN_API

/// declare the initialization routine for a plugin
#define PLUGIN_INIT() \
	const int PluginVersion = PLUGIN_API_VERSION; \
	PLUGIN_FUNC int PluginInit()

/// declare the cleanup routine for a plugin
#define PLUGIN_FREE() \
	PLUGIN_FUNC int PluginFree()

/// declare the display name a plugin
#define PLUGIN_DISPLAY_NAME(name) \
	PLUGIN_API const char * PluginDisplayName = name

/// declare the type of a plugin INPUT IN_OUT OUTPUT
#define PLUGIN_DISPLAY_TYPE(type) \
	PLUGIN_API const char * PluginType = type

/// declare the minimum required Plugin API version for a plugin
#define PLUGIN_MIN_VERSION(version) \
	PLUGIN_API const char * PluginMinVersion = version

/// declare the maximum supported Plugin API version for a plugin
#define PLUGIN_MAX_VERSION(version) \
	PLUGIN_API const char * PluginMaxVersion = version


/// The function signature for a routine that creates a Plugin
typedef IPlugin *(*PluginInitFunc)();
/// The function signature for a routine that destroys a Plugin
typedef void (*PluginFreeFunc)(IPlugin *);

/// A routine to let a plugin register a new Plugin type
CORE_FUNC void RegisterPlugin(const char *type, PluginInitFunc init_cb, PluginFreeFunc free_cb);


#endif
