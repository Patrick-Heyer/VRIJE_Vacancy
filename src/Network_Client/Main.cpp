/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///#include <iostream>
#include <map>
#include <pthread.h>

#include "pluginapi.h"
#include "Shared_Memory.h"

#include <cstdlib>
#include <iostream>

#include "NetThread.h"

#define PLUGIN_NAME "Network_Client"
#define PLUGIN_TYPE "OUTPUT"

/**
* @brief Network_Client plugin
*
* This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
* @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
* @date       jul 04, 2012
* @version    1.0
**/
class Network_Client : public IPlugin
{
public:
	void Main();
	bool loadConfiguration();
	bool saveConfiguration();
	bool Initialize_Output();
	bool Initialize_Input();
    
    void run();
	void stop();;
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Network_Client;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
	delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
	// register our new plugin
	RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
	RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
	return 0;
}
int lugar=1;

void Network_Client::Main()
{
	sleep(5);
	NetThread *net = new  NetThread();
	net->SetOutputStream(stdout);
	net->OpenOutputAddress("localhost", 2070);
	net->Write("send");
	while(running)
	{
		net->Read();
		if(net->GetStatus()>0)
		{
			string s = net->GetIncoming();
			if (s.find("recived")!=s.npos)
			{
				net->messages.clear();
				net->Write("send");
			}
		}
	}
	stoped=true;
}

bool Network_Client::loadConfiguration()
{
    return true;
}

bool Network_Client::saveConfiguration()
{
    return true;
}

bool Network_Client::Initialize_Output()
{

	return true;
}

bool Network_Client::Initialize_Input()
{
	return true;
}

void Network_Client::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}

void Network_Client::stop()
{
    running = false;
}
