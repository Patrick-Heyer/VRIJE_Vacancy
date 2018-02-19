///
/// \file   plugin.h
/// \author Patrick Heyer patrickhey@prodigy.net.mx
/// \brief  
///


#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
#include <iostream>
#include <pthread.h>

///
/// An abstract interface for plugins to create new plugins.
///
class IPlugin
{
public:
	pthread_t thread_id;
	bool running;
	bool stoped;
	
	/**
	 * @brief Initialize Output variables for the plugin to publish
	 *
	 * @return bool
	 **/
	virtual bool Initialize_Output() = 0;
	
	/**
	 * @brief Initialize Input variables for the plugin to publish
	 *
	 * @return bool
	 **/
	virtual bool Initialize_Input() = 0;
    
	/**
	 * @brief Main funcction of the plugin
	 *
	 * Started in a new thread by the Run() funcction is the most importatn funcction of the plugin.
	 * Infinite loop (generaly a while(running) function that runs undefenitly in a read/process/write cicle).
	 * @return void
	 **/
	virtual void Main() = 0;
	
    /**
	 * @brief Load configuration
     * 
     * The configuration can be loaded from file, or be hard coded into the plugin (not recomended), initialization of public and private variables should occur here
	 * 
     * By default loadConfiguration is called during PluginManager::Initialize() by the plugin manager.
     * 
	 * @return bool
	 **/
	virtual bool loadConfiguration() = 0;

     /**
	 * @brief Save configuration
     * 
     * The configuration should be saved to file, public and private variables that should be configured at startup should be saved.
     * 
     * By default saveConfiguration is called during PluginManager::Unload() by the plugin manager.
	 *
	 * @return bool
	 **/
	virtual bool saveConfiguration() = 0;

	/**
	 * @brief Execute plugin
	 *
	 * Starts a new thread and executes the plugins main() funcction.
	 * @return void
	 **/
	virtual void run()=0;
	
	/**
	 * @brief Stop plugin
	 *
	 * Stops the plugin 
	 * @return void
	 **/
	virtual void stop()=0;
	
	/**
	 * @brief Wrapps the Main function of the plugin object to ANSI C to be pased as a parameter to pthread_create.
     * 
     * This process alows plugins to be running in a new thread each (don't change this unless you know what you are doing)
	 *
	 * 
	 * @return static void
	 **/
	static void* IncWrapper(void* thisPtr) 
	{
	  ((IPlugin*) thisPtr)->Main();
	  return NULL;
	}
	
};

#endif

