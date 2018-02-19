#include "pluginapi.h"
#include "Shared_Memory.h"
#include "Logger.h"

/**
* @brief Color_Tracker plugin
*
* This plugin is an adaptation of Elias Ruiz color tracker for Gesture Therapy
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/

#define PLUGIN_NAME "Color_Tracker"
#define PLUGIN_TYPE "INPUT"

class Color_Tracker : public IPlugin
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
    double posX;
    double posY;
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Color_Tracker;
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
