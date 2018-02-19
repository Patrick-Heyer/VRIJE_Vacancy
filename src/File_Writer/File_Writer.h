#include "pluginapi.h"
#include "Shared_Memory.h"
#include "../Logger_API/Logger.h"

/**
* @brief File_Writer plugin
*
* This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
* @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
* @date       jul 04, 2012
* @version    1.0
**/
class File_Writer : public IPlugin
{
public:
    void Main();
    bool loadConfiguration();
    bool saveConfiguration();
    bool Initialize_Output();
    bool Initialize_Input();

    void run();
    void stop();

private:
    double *RShoulder_X;
    double *RShoulder_Y;
    double *RShoulder_Z;
    double *RElbow_X;
    double *RElbow_Y;
    double *RElbow_Z;
    double *RHand_X;
    double *RHand_Y;
    double *RHand_Z;

    double *LShoulder_X;
    double *LShoulder_Y;
    double *LShoulder_Z;
    double *LElbow_X;
    double *LElbow_Y;
    double *LElbow_Z;
    double *LHand_X;
    double *LHand_Y;
    double *LHand_Z;

    double *Head_X;
    double *Head_Y;
    double *Head_Z;
    double *Torso_X;
    double *Torso_Y;
    double *Torso_Z;

    double *BTGripper_X;
    double *BTGripper_Y;
    double *BTGripper_Z;

    double *LPMS_QUAT_W;
    double *LPMS_QUAT_X;
    double *LPMS_QUAT_Y;
    double *LPMS_QUAT_Z;

    double *LPMS_ACCEL_RAW_X;
    double *LPMS_ACCEL_RAW_Y;
    double *LPMS_ACCEL_RAW_Z;

    double *LPMS_GYRO_RAW_X;
    double *LPMS_GYRO_RAW_Y;
    double *LPMS_GYRO_RAW_Z;

    double *LPMS_MAT_11;
    double *LPMS_MAT_12;
    double *LPMS_MAT_13;
    double *LPMS_MAT_21;
    double *LPMS_MAT_22;
    double *LPMS_MAT_23;
    double *LPMS_MAT_31;
    double *LPMS_MAT_32;
    double *LPMS_MAT_33;

    double *LPMS_MAT_OFFSET_11;
    double *LPMS_MAT_OFFSET_12;
    double *LPMS_MAT_OFFSET_13;
    double *LPMS_MAT_OFFSET_21;
    double *LPMS_MAT_OFFSET_22;
    double *LPMS_MAT_OFFSET_23;
    double *LPMS_MAT_OFFSET_31;
    double *LPMS_MAT_OFFSET_32;
    double *LPMS_MAT_OFFSET_33;

	int User_ID;
};
