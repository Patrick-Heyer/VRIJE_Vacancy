/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///


#include <iostream>
#include <map>
#include <pthread.h>

#include "pluginapi.h"
#include "Shared_Memory.h"

#include <cstdlib>
#include <iostream>

#include "NetThread.h"

#define PLUGIN_NAME "Network_Server"

/**
* @brief Network_Server plugin
*
* This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
* @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
* @date       jul 04, 2012
* @version    1.0
**/
class Network_Server : public IPlugin
{
public:
	void Main();
	bool loadConfiguration();
	bool saveConfiguration();
	bool Initialize_Output();
	bool Initialize_Input();
    
    void run();
	void stop();
	
private:
	std::string phrase;
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Network_Server;
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
	return 0;
}

int contao=0;

void Network_Server::Main()
{
	NetThread *net = new NetThread();
	phrase=" ";
	net->SetOutputStream(stdout);
	net->OpenReadPort(2070);
	net->start();
	net->Accept();

	std::string respuesta;
	
	while(running)
	{
		net->Read();
		if(net->GetStatus()>0)
		{
			phrase = net->GetIncoming();
			std::cout << "client says " << phrase << std::endl;
			if(phrase=="Connecting")
			{
				net->messages.clear();
				net->Write("Connected");
			}
			if(phrase=="next_target")
			{
				net->messages.clear();
				net->Write("Target_Bed_Upstairs");
				std::cout << "enviado a Target_Bed_Upstairs" << std::endl;
			}
		}
		else
		{
			phrase=" ";
			
		}
		usleep(500);
		
	}
	stoped=true;

}


bool Network_Server::loadConfiguration()
{
    return true;
}

bool Network_Server::saveConfiguration()
{
    return true;
}

bool Network_Server::Initialize_Output()
{
	Shared_Memory::getInstance().Register_Output("Server_Phrase", &phrase);
	return true;
}

bool Network_Server::Initialize_Input()
{
	return true;
}

void Network_Server::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}

void Network_Server::stop()
{
    running = false;
}


