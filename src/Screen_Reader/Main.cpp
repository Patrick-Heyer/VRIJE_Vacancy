#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctime>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

#include "pluginapi.h"
#include "Shared_Memory.h"


#define PLUGIN_NAME "Screen_Reader"
#define PLUGIN_TYPE "INPUT"

using namespace cv;
using namespace std;

/**
 * @brief Screen_Reader plugin
 *
 * This plugin is a legacy wrapper that alows us to use Hocoma's games with this architecture
 * it writes output to a shared memory segment defined by hocoma that can not be changed
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 13, 2014
 * @version    1.0
 **/

class Screen_Reader : public IPlugin
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
    cv::Mat frame;
    cv::Mat frame_from_cam;
    cv::VideoCapture  cap;
    std::string *Plugin_ERROR;
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Screen_Reader;
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

struct ScreenShot
{
    ScreenShot(int x, int y, int width, int height):
    x(x),
    y(y),
    width(width),
    height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
        
        init = true;
    }
    
    void operator() (Mat& cvImg)
    {
        if(init == true)
            init = false;
        
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        
        cvImg = Mat(height, width, CV_8UC4, img->data);
    }
    
    ~ScreenShot()
    {
        if(init == false)
            XDestroyImage(img);
        
        XCloseDisplay(display);
    }
    
    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;
    
    bool init;
};

void Screen_Reader::Main()
{
    Mat img;
    while (running)
    {
        ScreenShot screen(0,0,1366,768);               
        screen(img);        
       // imshow("Screen", img);
        usleep(1000);
        
    }
    
    stoped = true;
    return;
}

bool Screen_Reader::loadConfiguration()
{
    return true;
}

bool Screen_Reader::saveConfiguration()
{
    return true;
}

bool Screen_Reader::Initialize_Output()
{	
    
    Shared_Memory::getInstance().Register_Output("Video_From_Screen", &frame);
    return true;
}

bool Screen_Reader::Initialize_Input()
{
    Plugin_ERROR = Shared_Memory::getInstance().Register_String_Input("Plugin_ERROR");
    return true;
}

void Screen_Reader::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
    stoped = false;
}

void Screen_Reader::stop()
{
    running = false;
}
