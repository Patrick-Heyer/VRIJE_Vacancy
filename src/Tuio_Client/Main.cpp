/*
*    <Tuio_Client sends data to Hocoma games from this architecture.>
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


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pluginapi.h"
#include "Shared_Memory.h"

#include <math.h>
#include <unistd.h>

#include <ctime>


#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>


#define PLUGIN_NAME "Tuio_Client"
#define PLUGIN_TYPE "INPUT"
#define PI 3.141592

#ifdef OPENGL_GUI
#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"
#include "../Gui/button.h"

#endif


#include "TuioDump.h"
		
void TuioDump::addTuioObject(TuioObject *tobj) {
	std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void TuioDump::updateTuioObject(TuioObject *tobj) {
	std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() 
				<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
}

void TuioDump::removeTuioObject(TuioObject *tobj) {
	std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void TuioDump::addTuioCursor(TuioCursor *tcur) {
	std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void TuioDump::updateTuioCursor(TuioCursor *tcur) {
	std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() 
				<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
}

void TuioDump::removeTuioCursor(TuioCursor *tcur) {
	std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void  TuioDump::refresh(TuioTime frameTime) {
	//std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

/**
* @brief Tuio_Client plugin
*
* This plugin is a legacy wrapper that alows us to use Hocoma's games with this architecture
* it writes output to a shared memory segment defined by hocoma that can not be changed
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/

class Tuio_Client : public IPlugin
{
public:
    void Main();
    bool loadConfiguration();
    bool saveConfiguration();
    bool Initialize_Output();
    bool Initialize_Input();
    void Init_Joy();

    void run();
    void stop();

private:
    double X_axis;
    double Y_axis;
    double Z_axis;

    double *RShoulder_X;


};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Tuio_Client;
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

cv::Mat edges,frame, HSV_image, HSV_image_local;




cv::Scalar hsvlow(147,107,62),hsvhigh(198,158,113);

void  getHSV(int event, int x, int y, int, void* );

void Tuio_Client::Main()
{
    running=true;
    stoped=false;
#ifdef OPENGL_GUI

    Gui::getInstance();
    Tab *pluginTab;

    pluginTab = new Tab("Tuio_Client");
    Gui::getInstance().setActiveTab("Tuio_Client");



#endif
int port = 3333;
	

	TuioDump dump;
	TuioClient client(port);
	client.addTuioListener(&dump);
	client.connect(false);
    


    while (running)
    {

    }
    client.disconnect();

    stoped = true;
    return;

}

bool Tuio_Client::loadConfiguration()
{
    return true;
}

bool Tuio_Client::saveConfiguration()
{
    return true;
}

bool Tuio_Client::Initialize_Output()
{
    Shared_Memory::getInstance().Register_Output("TUIO_0X", &X_axis);
    Shared_Memory::getInstance().Register_Output("TUIO_0Y", &Y_axis);
    return true;
}

bool Tuio_Client::Initialize_Input()
{
    RShoulder_X=Shared_Memory::getInstance().Register_Double_Input("RIGHT_SHOULDER_X");
    return true;
}

void Tuio_Client::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}

void Tuio_Client::stop()
{
    running = false;
}
