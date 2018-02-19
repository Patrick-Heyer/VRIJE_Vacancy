///
/// \file   pluginmanager.h
/// \author Patrick Heyer patrickhey@prodigy.net.mx
/// \brief  A Plugin Manager singleton
///

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "defines.h"

#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include "plugin.h"


/**
* @brief CORE_API PluginInstance
*
* An object to represent a single plugin in the system
* 
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/
class CORE_API PluginInstance
{
public:
    explicit PluginInstance(const std::string &name);
    ~PluginInstance();

    /// Load the plugin
    bool Load();
    /// Unload the plugin
    bool Unload();
    /// Return true if the plugin is loaded
    bool IsLoaded();

    /// Return the path to the plugin file on disk
    std::string GetFileName();
    /// Return the display name for the plugin
    std::string GetDisplayName();
    /// Pointer to the actual plugin instance
    IPlugin *actual;

private:
    /// Make this object be noncopyable as it holds a pointer
    PluginInstance(const PluginInstance &);
    const PluginInstance &operator =(const PluginInstance &);

    class Impl;
    Impl *mImpl;

};

/**
* @brief CORE_API PluginManager
*
* A manger for all plugins in the Core API using singleton pattern to avoid multiple instances
* 
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/
class CORE_API PluginManager
{
public:
    /// Return the single instance of the plugin manager
    static PluginManager &getInstance();

    /// Load a single plugin by name
    bool Load(const std::string &name);
    
    /// load a list of plugins by name from file
    bool LoadFromFile(const std::string &filename);
    
    /// Initialize a single plugin by name    
    bool Initialize(const std::string &name);
    
    ///Initialize all plugins in mPlugins  
    bool InitializeAll();

    /// Execute a single plugin by name
    bool Execute(const std::string &name);

    /// Execute all plugins in mPlugins
    bool ExecuteAll();    
    
    /// Unload a single plugin by name
    bool Unload(const std::string &name);

    /// Unload all plugins
    bool UnloadAll();


private:
    ///Private constructor to force the use of getInstance which creates one and only one instance and returns this unique instance at any moment 
    PluginManager();
    ~PluginManager();
    PluginManager(const PluginManager &);
    const PluginManager &operator =(const PluginManager &);
    typedef std::list<PluginInstance *>   PluginList;
    typedef PluginList::iterator   TListItor;
    PluginList                     m_pluginlist;    
    static PluginManager          *mPlugins; // singleton
};

#endif
