/*
*    <Function_Test sends data to Hocoma games from this architecture.>
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

#include "Function_Test.h"

INITIALIZE_NULL_EASYLOGGINGPP

#define PLUGIN_NAME "Function_Test"
#define PLUGIN_TYPE "INPUT"

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Function_Test;
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

void empty_function(std::vector<std::string> i_params, std::vector<std::string> o_params)
{
    std::cout << "Empty function" << std::endl;
    std::cout << "INPUT" << std::endl; 
    for(auto i : i_params) 
    {
        std::cout << i << std::endl; 
    }
    std::cout << "OUTPUT" << std::endl; 
    for(auto i : o_params) 
    {
        std::cout << i << std::endl; 
    }
}

void input_function(std::vector<std::string> i_params, std::vector<std::string> o_params)
{
    std::cout << "Input function" << std::endl;
    std::cout << "INPUT" << std::endl; 
    for(auto i : i_params) 
    {
        std::cout << i << std::endl; 
    }
    std::cout << "OUTPUT" << std::endl; 
    for(auto i : o_params) 
    {
        std::cout << i << std::endl; 
    }
}

void output_function(std::vector<std::string> i_params, std::vector<std::string> o_params)
{
    std::cout << "Output function" << std::endl;
    std::cout << "INPUT" << std::endl; 
    for(auto i : i_params) 
    {
        std::cout << i << std::endl; 
    }
    std::cout << "OUTPUT" << std::endl; 
    for(auto i : o_params) 
    {
        std::cout << i << std::endl; 
    }
}

void IO_function(std::vector<std::string> i_params, std::vector<std::string> o_params)
{
    std::cout << "IO function" << std::endl;
    std::cout << "INPUT" << std::endl; 
    for(auto i : i_params) 
    {
        std::cout << i << std::endl; 
    }
    std::cout << "OUTPUT" << std::endl; 
    for(auto i : o_params) 
    {
        std::cout << i << std::endl; 
    }
}

void Function_Test::Main()
{
    return;
}

bool Function_Test::loadConfiguration()
{
	return true;
}

bool Function_Test::saveConfiguration()
{
	return true;
}

bool Function_Test::Initialize_Output()
{
    std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)> published_empty = empty_function;
    Shared_Memory::getInstance().Register_Output("published_empty_function", published_empty);
    
    std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)> published_input = input_function;
    Shared_Memory::getInstance().Register_Output("published_input_function", published_input);
    
    std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)> published_output = output_function;
    Shared_Memory::getInstance().Register_Output("published_output_function", published_output);
    
    std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)> published_IO = IO_function;
    Shared_Memory::getInstance().Register_Output("published_IO_function", published_IO);
	return true;
}

bool Function_Test::Initialize_Input()
{
	return true;
}
void Function_Test::run()
{
	stoped = true;
}

void Function_Test::stop()
{
	running = false;
}
