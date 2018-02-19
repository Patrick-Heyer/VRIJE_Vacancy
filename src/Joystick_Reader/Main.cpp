/*
*    <Joystick_Reader sends data to Hocoma games from this architecture.>
*    Copyright (C) 2013  Patrick Heyer Wollenberg <patrickhey@prodigy.net.mx>
*
*    This library is free software; you can redistribute it and/or
*    modify it under the terms of the GNU Lesser General Public
*    License as published by the Free Software Foundation; either
*    version 2.1 of the License, or (at your option) any later version.
*
*    This library is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with this library; if not, write to the Free Software
*    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <unistd.h>

#include "Joypad.h"
#include "Joystick_Reader"


#define PLUGIN_NAME "Joystick_Reader"
#define PLUGIN_TYPE "INPUT"

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Joystick_Reader;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
	delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);
PLUGIN_DISPLAY_TYPE(PLUGIN_TYPE);

PLUGIN_INIT()
{
	// register our new plugin
	RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
	return 0;
}

//------------------------------------------------------------	OnInit()
//
int Joystick_Reader::Init_Joy() {

	// first initialise Direct Input or the linux joystick lib
	if (!InitInput()) {
		std::cerr << "Failed to initialise the joypad lib\n";
		return 0;
	}

	// make sure there is a joypad attached
	if (NumJoypads() == 0) {
		std::cerr << "No Joypad attached\n";
		return 0;
	}

	// make sure there is a joypad attached
	if (NumJoypads() == 1) {
		std::cerr << "Only virtual joystick detected\n";
		return 0;
	}

	int joy=1;
	// get the first available jopypad
	if( !(g_pJoypad = GetJoypad(joy)) ) {
		std::cerr << "Failed to access joypad\n";
		return 0;
	}
	return 1;
}

void Joystick_Reader::Main()
{

    if(!Init_Joy())
    {
        LOG(WARNING) << "Joystick device not connected or with wrong permissions.";
        LOG(ERROR) << PluginDisplayName << " will NOT keep executing";
        stoped=true;
        return;
    }
	while(running)
	{
        UpdateInputState();
		X_axis=(float)g_pJoypad->Xaxis()/32766;
		Y_axis=(float)g_pJoypad->Yaxis()/32766;
		Z_axis=(float)g_pJoypad->Zaxis()/32766;
		U_axis=(float)g_pJoypad->Xrot()/32766;
		V_axis=(float)g_pJoypad->Yrot()/32766;
		W_axis=(float)g_pJoypad->Zrot()/32766;
		//std::cout << X_axis << " " << Y_axis << " " << Z_axis << " " << RX_axis << " " << RY_axis << " " << RZ_axis  << std::endl;
	}	
	stoped=true;
    return;	
}

bool Joystick_Reader::loadConfiguration()
{
	return true;
}

bool Joystick_Reader::saveConfiguration()
{
   
	return true;
}

bool Joystick_Reader::Initialize_Output()
{
	Shared_Memory::getInstance().Register_Output("Position_For_Game_X", &X_axis);
	Shared_Memory::getInstance().Register_Output("Position_For_Game_Y", &Y_axis);
	Shared_Memory::getInstance().Register_Output("Position_For_Game_Z", &Z_axis);
    Shared_Memory::getInstance().Register_Output("Position_For_Game_U", &U_axis);
    Shared_Memory::getInstance().Register_Output("Position_For_Game_V", &V_axis);
	Shared_Memory::getInstance().Register_Output("Position_For_Game_W", &W_axis);
	return true;
}

bool Joystick_Reader::Initialize_Input()
{
	return true;
}

void Joystick_Reader::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}

void Joystick_Reader::stop()
{
    running = false;
}
