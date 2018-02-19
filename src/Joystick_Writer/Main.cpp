#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
    
#include "Joystick_Writer.h"
    
INITIALIZE_NULL_EASYLOGGINGPP

#define PLUGIN_NAME "Joystick_Writer"
#define PLUGIN_TYPE "OUTPUT"

int do_uinput(int fd, unsigned short key, int pressed, unsigned short event_type) {
    struct uinput_event event;
    memset(&event, 0 , sizeof(event));

    event.type = event_type;

    event.code = key;
    //this is done to solve a "bug" that seems to transform ints from 64 to 32
    //or something i dont know for shure but it solves the problem on a 64 bit machine
    if(event_type==EV_ABS)
    {
    event.value = pressed*0.015625;
    }
    else
    {
      event.value = pressed;
    }

    write(fd,&event,sizeof(event));

    return true;
}

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Joystick_Writer;
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

void Joystick_Writer::Main()
{
    struct uinput_user_dev uidev;

    int                    i;
    int                    fd;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Virtual_joystick");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    ioctl (fd, UI_SET_EVBIT, EV_SYN);
    ioctl (fd, UI_SET_EVBIT, EV_ABS);
    ioctl (fd, UI_SET_EVBIT, EV_KEY);

    ioctl (fd, UI_SET_ABSBIT, ABS_X);
    ioctl (fd, UI_SET_ABSBIT, ABS_Y);
    ioctl (fd, UI_SET_ABSBIT, ABS_Z);
    ioctl (fd, UI_SET_ABSBIT, ABS_RX);
    ioctl (fd, UI_SET_ABSBIT, ABS_RY);
    ioctl (fd, UI_SET_ABSBIT, ABS_RZ);
    ioctl (fd, UI_SET_ABSBIT, ABS_RT);
    ioctl (fd, UI_SET_ABSBIT, ABS_MAX);

    for(i = 0; i < ABS_MAX; ++i)
    {
        uidev.absmax[i] = 511;
        uidev.absmin[i] = -512;
    }

    // Create device
    write (fd, &uidev, sizeof (uidev));
    if (ioctl (fd, UI_DEV_CREATE))
    {
        LOG(WARNING) << "Unable to create UIDEVUT device.";
        LOG(ERROR) << PluginDisplayName << " will NOT keep executing";
        stop();
	running=false;
    stoped = true;
	return;
    }

    while(running)
    {
        do_uinput(fd,ABS_X,*X_axis*32767, EV_ABS);
        do_uinput(fd,ABS_Y,*Y_axis*32767, EV_ABS);
        do_uinput(fd,ABS_Z,*Z_axis*32767, EV_ABS);
        do_uinput(fd,ABS_RX,*RX_axis*32767, EV_ABS);
        do_uinput(fd,ABS_RY,*RY_axis*32767, EV_ABS);
        do_uinput(fd,ABS_RZ,*RZ_axis*32767, EV_ABS);
    }
    stoped = true;
    return;
}

bool Joystick_Writer::loadConfiguration()
{
    return true;
}

bool Joystick_Writer::saveConfiguration()
{
    return true;
}

bool Joystick_Writer::Initialize_Output()
{
    return true;
}

bool Joystick_Writer::Initialize_Input()
{
    X_axis=Shared_Memory::getInstance().Register_Double_Input("Tracker_X");
    Y_axis=Shared_Memory::getInstance().Register_Double_Input("Tracker_Y");
    Z_axis=Shared_Memory::getInstance().Register_Double_Input("Tracker_Z");
    RX_axis=Shared_Memory::getInstance().Register_Double_Input("Position_For_Game_RX");
    RY_axis=Shared_Memory::getInstance().Register_Double_Input("Position_For_Game_RY");
    RZ_axis=Shared_Memory::getInstance().Register_Double_Input("Position_For_Game_RZ");
    return true;
}

void Joystick_Writer::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}


void Joystick_Writer::stop()
{
    running = false;
}
