/// 
/// \file   coreapi.h
/// \author Patrick Heyer patrickhey@prodigy.net.mx
/// \brief  A factory object to create Renderer instances.
///

#ifndef COREAPI_H
#define COREAPI_H

#include "defines.h"
#include "plugin.h"

#include <string>
#include <map>

///
/// A factory object in the Core API
///
class CORE_API PluginFactory
{
public:
	typedef IPlugin *(*CreateCallback)();

	static void RegisterPlugin(const std::string &type, CreateCallback cb);
	static void UnregisterPlugin(const std::string &type);
	static IPlugin *CreatePlugin(const std::string &type);

private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mPlugins;
};

#endif
