/*
*    <Function_Caller sends data to Hocoma games from this architecture.>
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

#include "Function_Caller.h"

INITIALIZE_NULL_EASYLOGGINGPP

#define PLUGIN_NAME "Function_Caller"
#define PLUGIN_TYPE "INPUT"

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Function_Caller;
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

std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>  empty_call;
std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>  input_call;
std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>  output_call;
std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>  IO_call;

void Function_Caller::Main()
{
    std::vector<std::string> i_empty;
    std::vector<std::string> o_empty;
    std::vector<std::string> i_input= {"INPUT_1", "INPUT_2", "INPUT_3", "INPUT_4"};
    std::vector<std::string> o_input= {"OUTPUT_1", "OUTPUT_2", "OUTPUT_3", "OUTPUT_4"};
    
    for(int o=0; o<10000; o++)
    {
        usleep(500);
    }
    
    empty_call(i_empty,o_empty);
    input_call(i_input,o_empty);
    output_call(i_empty,o_input);
    IO_call(i_input,o_input);

    stoped=true;
    return;
}


bool Function_Caller::loadConfiguration()
{
	return true;
}

bool Function_Caller::saveConfiguration()
{
	return true;
}

bool Function_Caller::Initialize_Output()
{

	return true;
}

bool Function_Caller::Initialize_Input()
{
    empty_call = Shared_Memory::getInstance().Register_Func_Input("published_empty_function");
    input_call = Shared_Memory::getInstance().Register_Func_Input("published_input_function");
    output_call = Shared_Memory::getInstance().Register_Func_Input("published_output_function");
    IO_call = Shared_Memory::getInstance().Register_Func_Input("published_IO_function");
	return true;
}
void Function_Caller::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}

void Function_Caller::stop()
{
	running = false;
}
