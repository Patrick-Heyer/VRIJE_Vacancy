#include "pluginapi.h"
#include "Shared_Memory.h"
#include "Logger.h"

/**
* @brief Camera_Reader plugin
*
* This plugin reads the input from the webcam (index 0 unless secefied in config) and publishes it as a cv::Mat under the name "Video_From_Camera"
* 
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/

#define PLUGIN_NAME "Camera_Reader"
#define PLUGIN_TYPE "INPUT"

class Camera_Reader : public IPlugin
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
    int Camera_index;
    int Visible;
	cv::Mat frame_from_cam;
	cv::VideoCapture  cap;
	std::string *Plugin_ERROR;
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Camera_Reader;
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
