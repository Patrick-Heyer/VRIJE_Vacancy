#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <functional>


#include "ConfigFile.h"
#include "Triplet.h"

#if _WIN32
#define SHARED_MEMORY_API __declspec( dllexport )
#else
#define SHARED_MEMORY_API 
#endif

using namespace std::placeholders;


class SHARED_MEMORY_API Shared_Memory
{

public:
	void   Initialize(void);
	static Shared_Memory &getInstance();
	static void freeInstance(void);

	void Register_Output(std::string name, bool *value);
	bool* Register_Bool_Input(std::string name);
	void Register_Output(std::string name, char *value);
	char* Register_Char_Input(std::string name);
	void Register_Output(std::string name, double *value);
	double* Register_Double_Input(std::string name);
	void Register_Output(std::string name, float *value);
	float* Register_Float_Input(std::string name);
	void  Register_Output(std::string name, int *value);
	int*  Register_Int_Input(std::string name);
	void  Register_Output(std::string name, long *value);
	long* Register_Long_Input(std::string name);
	void  Register_Output(std::string name, std::string *value);
	std::string * Register_String_Input(std::string name);
	void  Register_Output(std::string name, void *value);
	void * Register_Void_Input(std::string name);
    
    void Register_Output(std::string name, std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)> value);
    std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>  Register_Func_Input(std::string name);
	
	std::map <std::string, double *> List_Doubles;
	std::map <std::string, float *> List_Floats;
	std::map <std::string, int *> List_Ints;
	std::map <std::string, long *> List_Longs;
	std::map <std::string, bool *> List_Bools;
	std::map <std::string, char *> List_Chars;
	std::map <std::string, std::string *> List_Strings;
	std::map <std::string, void *> List_Voids;
    std::map <std::string, std::function<void(std::vector<std::string> i_params, std::vector<std::string> o_params)>> List_Funcs;

protected:
	Shared_Memory();
	Shared_Memory(const Shared_Memory& other);
	virtual ~Shared_Memory();
	virtual Shared_Memory& operator=(const Shared_Memory& other);

private:
	static Shared_Memory *Singleton;
};

