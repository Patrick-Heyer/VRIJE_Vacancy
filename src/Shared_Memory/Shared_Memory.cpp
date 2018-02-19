#include <iostream>
#include <fstream>


#include "Shared_Memory.h"
#include "Logger.h"

INITIALIZE_NULL_EASYLOGGINGPP

Shared_Memory	*Shared_Memory::Singleton = 0;
std::string error;

double default_double=0;
float default_float=0;
int default_int=0;
long default_long=0;
bool debault_bool=false;
char default_char=' ';
std::string default_string=" ";

Shared_Memory::Shared_Memory()
{

}

Shared_Memory::Shared_Memory(const Shared_Memory& other)
{

}

Shared_Memory::~Shared_Memory()
{

}

Shared_Memory& Shared_Memory::operator=(const Shared_Memory& other)
{
	return *this;
}

void Shared_Memory::Initialize()
{
	el::Helpers::setStorage(sharedLoggingRepository());

	el::Configurations conf("../conf/logger.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	LOG(TRACE) << "Shared Memory Initialized";
}

Shared_Memory& Shared_Memory::getInstance(void )
{
	if ( Singleton == 0 )
	{
		Singleton = new Shared_Memory;
		Singleton->Initialize();

		Singleton->Register_Output("default", &default_double);
		Singleton->Register_Output("default", &default_float);
		Singleton->Register_Output("default", &default_int);
		Singleton->Register_Output("default", &default_long);
		Singleton->Register_Output("default", &debault_bool);
		Singleton->Register_Output("default", &default_char);
		Singleton->Register_Output("default", &default_string);
	}
	return *Singleton;
}

void Shared_Memory::freeInstance(void )
{
	if ( Singleton != 0 )
	{
		delete Singleton;
		Singleton = 0;
	}
}

void Shared_Memory::Register_Output(std::string name, double *value)
{
	getInstance().List_Doubles.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Double List" ;
}

void Shared_Memory::Register_Output(std::string name, float *value)
{
	getInstance().List_Floats.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Float List";
}

void Shared_Memory::Register_Output(std::string name, int *value)
{
	getInstance().List_Ints.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Int List";
}

void Shared_Memory::Register_Output(std::string name, long int* value)
{
	getInstance().List_Longs.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Long List";
}

void Shared_Memory::Register_Output(std::string name, bool* value)
{
	getInstance().List_Bools.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Bool List";
}

void Shared_Memory::Register_Output(std::string name, char* value)
{
	getInstance().List_Chars.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Char List";
}

void Shared_Memory::Register_Output(std::string name, std::string* value)
{
	getInstance().List_Strings.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in String List";
}

void Shared_Memory::Register_Output(std::string name, void *value)
{
	getInstance().List_Voids.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in Void List";
}

void Shared_Memory::Register_Output(std::string name, std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)> value)
{
    getInstance().List_Funcs.insert(std::make_pair(name, value));
	LOG(TRACE) << "registerd " << name << " in FUNCTION List";
}

double * Shared_Memory::Register_Double_Input(std::string name)
{
	if(getInstance().List_Doubles.find(name) != getInstance().List_Doubles.end())
	{
		LOG(TRACE) << name << " found in Double list";
		return  getInstance().List_Doubles.find(name)->second;
	}
	else 
	{		
		error="Variable "+name+" not found in double list USING DEFAULT DOUBLE";
		LOG(WARNING) << error;
		return  getInstance().List_Doubles.find("default")->second;
	}
}

float * Shared_Memory::Register_Float_Input(std::string name)
{

	if(getInstance().List_Floats.find(name) != getInstance().List_Floats.end())
	{
		LOG(TRACE) << name << " found in Float list";
		return  getInstance().List_Floats.find(name)->second;
	}
	else 
	{
		error = "Variable " + name + " not found in float list USING DEFAULT FLOAT";
		LOG(WARNING) << error;
		return  getInstance().List_Floats.find("default")->second;
	}
}

int * Shared_Memory::Register_Int_Input(std::string name)
{

	if (getInstance().List_Ints.find(name) != getInstance().List_Ints.end())
	{
		LOG(TRACE) << name << " found in Ints list";
		return  getInstance().List_Ints.find(name)->second;
	}
	else
	{
		error = "Variable " + name + " not found in ints list USING DEFAULT INT";
		LOG(WARNING) << error;
		return  getInstance().List_Ints.find("default")->second;
	}
}

long * Shared_Memory::Register_Long_Input(std::string name)
{

	if (getInstance().List_Longs.find(name) != getInstance().List_Longs.end())
	{
		LOG(TRACE) << name << " found in Ints list";
		return  getInstance().List_Longs.find(name)->second;
	}
	else
	{
		error = "Variable " + name + " not found in longs list USING DEFAULT LONG";
		LOG(WARNING) << error;
		return  getInstance().List_Longs.find("default")->second;
	}
}

bool * Shared_Memory::Register_Bool_Input(std::string name)
{

	if (getInstance().List_Bools.find(name) != getInstance().List_Bools.end())
	{
		LOG(TRACE) << name << " found in Ints list";
		return  getInstance().List_Bools.find(name)->second;
	}
	else
	{
		error = "Variable " + name + " not found in longs list USING DEFAULT LONG";
		LOG(WARNING) << error;
		return  getInstance().List_Bools.find("default")->second;
	}
}

char * Shared_Memory::Register_Char_Input(std::string name)
{
	if (getInstance().List_Chars.find(name) != getInstance().List_Chars.end())
	{
		LOG(TRACE) << name << " found in Chars list";
		return  getInstance().List_Chars.find(name)->second;
	}
	else
	{
		error = "Variable " + name + " not found in chars list USING DEFAULT CHAR";
		LOG(WARNING) << error;
		return  getInstance().List_Chars.find("default")->second;
	}
}

std::string* Shared_Memory::Register_String_Input(std::string name)
{
	if(getInstance().List_Strings.find(name) != getInstance().List_Strings.end())
	{
		LOG(TRACE) << name << " found in String list";
		return  getInstance().List_Strings.find(name)->second;
	}
	else 
	{
		error = "Variable " + name + " not found in string list USING DEFAULT STRING";
		LOG(WARNING) << error;
		return  getInstance().List_Strings.find("default")->second;
	}
}

void* Shared_Memory::Register_Void_Input(std::string name)
{
	if(getInstance().List_Voids.find(name) != getInstance().List_Voids.end())
	{
		LOG(TRACE) << name << " found in Void list";
		return  getInstance().List_Voids.find(name)->second;
	}
	else 
	{
		error = "Variable " + name + " not found in void list USING DEFAULT NULL POINTER";
		LOG(WARNING) << error;
		return  NULL;
	}
}

std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>  Shared_Memory::Register_Func_Input(std::string name)
{
	if(getInstance().List_Funcs.find(name) != getInstance().List_Funcs.end())
	{
		LOG(TRACE) << name << " found in FUNCTION list";
        return getInstance().List_Funcs.find(name)->second;
		//return  getInstance().List_Funcs.find(name)->second;
	}
	else 
	{
		error = "FUNCTION " + name + " not found in void list USING DEFAULT NULL POINTER";
		LOG(WARNING) << error;
		return  NULL;
	}
}


