
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

#include "Camera_Reader.h"

INITIALIZE_NULL_EASYLOGGINGPP

void Camera_Reader::Main()
{
	cap.open(Camera_index); 
	if (!cap.isOpened())  // if not success, exit program
	{
		LOG(ERROR) << "Can not open the web cam";
		*Plugin_ERROR = "NO_CAMERA";
		return;
	}
	LOG(TRACE) << "Web cam connected";
		
	while (running)
	{
		bool bSuccess = cap.read(frame_from_cam); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			LOG(WARNING) << "Cannot read a frame from video stream";
		}
		else 
		{
            if(Visible==1)
            {
                imshow("Cam", frame_from_cam);		
                
            }
		}

	}
	cap.release();
	stoped = true;
	return;
}

bool Camera_Reader::loadConfiguration()
{
    std::stringstream path;
    path <<"../conf/" << PLUGIN_NAME;
    ConfigFile config( path.str() );
	config.readInto(Camera_index, "Camera_index");
    config.readInto(Visible, "Visible");
	return true;
}

bool Camera_Reader::saveConfiguration()
{
    std::stringstream path;
    path <<"../conf/" << PLUGIN_NAME;
    ConfigFile config( path.str() );
	config.add("Camera_index", Camera_index);
    config.add("Visible", Visible);
    std::ofstream file(path.str());
	file << config;
	file.close();
	return true;
}

bool Camera_Reader::Initialize_Output()
{	
	Shared_Memory::getInstance().Register_Output("Video_From_Camera", &frame_from_cam);
	return true;
}

bool Camera_Reader::Initialize_Input()
{
	Plugin_ERROR = Shared_Memory::getInstance().Register_String_Input("Plugin_ERROR");
	return true;
}

void Camera_Reader::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
	running = true;
	stoped = false;
}

void Camera_Reader::stop()
{
	running = false;
}
