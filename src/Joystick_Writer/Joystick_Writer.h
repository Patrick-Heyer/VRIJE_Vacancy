#include "pluginapi.h"
#include "Shared_Memory.h"
#include "../Logger_API/Logger.h"

/**
* @brief Joystick_Writer plugin
*
* This plugin is a legacy wrapper that alows us to use Hocoma's games with this architecture
* it writes output to a shared memory segment defined by hocoma that can not be changed
* @author     Patrick Heyer, patrickhey@prodigy.net.mx
* @date       jul 13, 2014
* @version    1.0
**/

class Joystick_Writer : public IPlugin
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
    double *X_axis;
    double *Y_axis;
    double *Z_axis;
    double *RX_axis;
    double *RY_axis;
    double *RZ_axis;
    double *POV;
};

#define die(str, args...) do { \
        perror(str); \
         \
    } while(0)

#define ABS_RT		0x1d
    
struct uinput_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    int value;
};
