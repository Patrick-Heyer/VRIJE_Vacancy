#include "coreapi.h"
#include "pluginapi.h"

void RegisterPlugin(const char *type, PluginInitFunc init_cb, PluginFreeFunc free_cb)
{
	PluginFactory::RegisterPlugin(type, init_cb);
	(void) free_cb; // we never free these objects
}

