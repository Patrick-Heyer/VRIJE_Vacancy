#include "pluginapi.h"
#include "Shared_Memory.h"
#include "Logger.h"

/**
* @brief Function_Test plugin
*
* This plugin is a test to add function calls to the Shared_Memory, the functions have 2 std::vectors string as parameters.
* Each function is responsible for parsing the imput to map it to shared variables
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/
class Function_Test : public IPlugin
{
public:
	void Main();
	bool loadConfiguration();
	bool saveConfiguration();
	bool Initialize_Output();
	bool Initialize_Input();
	void stop();
	void run();
    
private:
};
