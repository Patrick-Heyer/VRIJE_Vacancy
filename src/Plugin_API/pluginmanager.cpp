#include <string>
#include <map>
#include <set>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>

#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "pthread.h"
#include "pluginmanager.h"
#include "coreapi.h"
#include "../Logger_API/Logger.h"

INITIALIZE_NULL_EASYLOGGINGPP

class PluginInstance::Impl
{
public:
	std::string mFileName;
	std::string mDisplayName;
#if _WIN32
	// Plugin loading routines for Windows
	HINSTANCE handle;
	typedef FARPROC PluginFunc;

	bool Load()
	{
		std::wstring stemp = std::wstring(mFileName.begin(), mFileName.end());
		LPCWSTR sw = stemp.c_str();
		handle = LoadLibrary(sw);
		return (handle != NULL);
	}

	bool Unload()
	{
		if (!handle)
			return true;
		return FreeLibrary(handle) != 0;
	}

	PluginFunc GetFunction(const std::string &name)
	{
		PluginFunc fptr = GetProcAddress(handle, name.c_str());
		if (! fptr)
			return NULL;
		return fptr;
	}

#else
	// Plugin loading routines for Mac & Linux
	void *handle;

	typedef void(*PluginFunc)();

	bool Load()
	{
		handle = dlopen(mFileName.c_str(), RTLD_LOCAL | RTLD_LAZY);
		return (handle != NULL);
	}

	bool Unload()
	{
		if (!handle)
			return true;
		return dlclose(handle) == 0;
	}

	PluginFunc GetFunction(const std::string &name)
	{
		dlerror();
		PluginFunc fptr = (PluginFunc) dlsym(handle, name.c_str());
		if (dlerror())
			return NULL;
		return fptr;
	}
#endif
};

PluginInstance::PluginInstance(const std::string &name)
{
	mImpl = new Impl;
	mImpl->mDisplayName = name;

#if _WIN32
	mImpl->mFileName = name + ".dll";
#elif __APPLE__
	mImpl->mFileName = "lib" + name + ".dylib";
#else
	mImpl->mFileName = name;
#endif
}

PluginInstance::~PluginInstance()
{
	delete mImpl;
}

bool PluginInstance::Load()
{
	if (! mImpl->Load())
		return false;

	Impl::PluginFunc init_func = mImpl->GetFunction("PluginInit");
	if (!init_func)
		return false;

	(*init_func)();

	Impl::PluginFunc name_string = mImpl->GetFunction("PluginDisplayName");
	if (name_string)
	{
		const char **ptr = (const char **) name_string;
		mImpl->mDisplayName = *ptr;
	}
	return true;
}

bool PluginInstance::Unload()
{
	return mImpl->Unload();
}

bool PluginInstance::IsLoaded()
{
	return (mImpl->handle != NULL);
}

std::string PluginInstance::GetFileName()
{
	return mImpl->mFileName;
}

std::string PluginInstance::GetDisplayName()
{
	return mImpl->mDisplayName;
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

PluginManager &PluginManager::getInstance()
{
	static PluginManager mgr;
	return mgr;
}

bool PluginManager::Load(const std::string &filename)
{
	std::string temp;
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if((*itor)->GetFileName().find_last_of(".") != std::string::npos)
		{
			temp=(*itor)->GetFileName().substr(0,(*itor)->GetFileName().find_last_of("."));
		}

		if( strcmp( temp.substr(3).c_str(), filename.c_str() ) == 0 )
		{
            LOG(WARNING) << temp.substr(3) << " Plugin alredy loaded";
			return true;
		}
	}
	// try to load this plugin
	PluginInstance *pi = new PluginInstance(filename);
	if (! pi->Load())
	{
		delete pi;
		LOG(ERROR) << filename << " Plugin could not be loaded Sorry!!!!";
		return false;
	}

	// success! add the plugin to our manager
	m_pluginlist.push_back(pi);
	LOG(TRACE) << "Plugin loaded "<< pi->GetDisplayName();

	return true;
}

bool PluginManager::LoadFromFile(const std::string &filename)
{
	std::ifstream inFile;
	std::string plugin_name;

	inFile.open(filename.c_str());
	if (!inFile) {
		LOG(WARNING) << "Unable to open plugin file " << filename.c_str();
		return false;
	}

	LOG(TRACE) << "Opened plugin file " << filename.c_str();

	while (inFile >> plugin_name) {
		Load(plugin_name);
	}

	inFile.close();

	return true;
}

bool PluginManager::Initialize(const std::string& name)
{
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) == 0 )
		{
			(*itor)->actual=PluginFactory::CreatePlugin(name.c_str());
            LOG(TRACE) <<"Created plugin " << (*itor)->GetDisplayName();
		}
	}
	
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) == 0 )
		{
			(*itor)->actual->Initialize_Output();
            LOG(TRACE) <<"Plugin OUTPUT initialized for " << (*itor)->GetDisplayName();

		}
	}

	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) == 0 )
		{
			(*itor)->actual->Initialize_Input();
            LOG(TRACE) <<"Plugin INPUT initialized for " << (*itor)->GetDisplayName();
		}
	}
	
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) == 0 )
		{
            LOG(TRACE) <<"Configuration loaded for " << (*itor)->GetDisplayName();
			(*itor)->actual->loadConfiguration();

		}
	}
	return true;
}

bool PluginManager::InitializeAll()
{
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		(*itor)->actual=PluginFactory::CreatePlugin((*itor)->GetDisplayName().c_str());
		LOG(TRACE) <<"Created plugin " << (*itor)->GetDisplayName();
	}
	
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		(*itor)->actual->Initialize_Output();
        LOG(TRACE) <<"Plugin OUTPUT initialized for " << (*itor)->GetDisplayName();
	}

	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		(*itor)->actual->Initialize_Input();
		LOG(TRACE) <<"Plugin INPUT Initialized for " << (*itor)->GetDisplayName();
	}
	
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		LOG(TRACE) <<"Configuration loaded for " << (*itor)->GetDisplayName();
		(*itor)->actual->loadConfiguration();
	}
	return true;
}

bool PluginManager::Execute(const std::string& name)
{

	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) == 0 )
		{
			(*itor)->actual->run();
            LOG(TRACE) <<"Started to run plugin " << (*itor)->GetDisplayName();
			return true;
		}
	}
	return false;
}

bool PluginManager::ExecuteAll()
{
	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		(*itor)->actual->run();
        LOG(TRACE) <<"Started to run plugin " << (*itor)->GetDisplayName();
	}
	return true;
}

bool PluginManager::Unload(const std::string &name)
{

	for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		if (strcmp((*itor)->GetDisplayName().c_str(), name.c_str()) == 0)
		{
			LOG(TRACE) <<"Stoping plugin " << (*itor)->GetDisplayName();
			do
			{
                (*itor)->actual->stop();
			} while ((*itor)->actual->stoped == false);
            (*itor)->actual->saveConfiguration();
            (*itor)->~PluginInstance();
		}
	}
	return false;
}

bool PluginManager::UnloadAll()
{
    for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
	{
		LOG(TRACE) <<"Stoping plugin " << (*itor)->GetDisplayName();
			do
			{
                (*itor)->actual->stop();
			} while ((*itor)->actual->stoped == false);
            (*itor)->actual->saveConfiguration();
            (*itor)->~PluginInstance();
	}
	return true;

}


