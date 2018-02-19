/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include <unistd.h>

#include "File_Writer.h"

#include "../Gui/gui.h"
#include "../Gui/input_singleton.h"


#define PLUGIN_NAME "File_Writer"
#define PLUGIN_TYPE "OUTPUT"


PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new File_Writer;
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

int counter=1;
bool recording=false;
std::string name = "../data/Capture/Subject_";

void File_Writer::Main()
{
	#ifdef OPENGL_GUI
	
	Gui::getInstance();
	Tab *pluginTab;
	
	pluginTab = new Tab("File_Writer");
	Gui::getInstance().setActiveTab("File_Writer");
	#endif
	
    std::ofstream myfile;


    while(running)
    {
        if(InputSingleton::getInstance().key==32)
        {
            InputSingleton::getInstance().key=NULL;
            if(recording==false)
            {
                recording=true;
                std::cout << "Start Recording " << std::endl;
                std::stringstream sstm;
                sstm << name << User_ID << "_" << counter << ".csv";
                counter++;

                myfile.open (sstm.str().c_str(), std::ios::out| std::ios::trunc );
                sstm.str(" ");
                myfile << "RShoulder_X,RShoulder_Y,RShoulder_Z,RElbow_X,RElbow_Y,RElbow_Z ,RHand_X,RHand_Y,RHand_Z," <<
                       "LShoulder_X,LShoulder_Y,LShoulder_Z,LElbow_X,LElbow_Y,LElbow_Z ,LHand_X,LHand_Y,LHand_Z," <<
                       "Torso_X,Torso_Y,Torso_Z,Head_X,Head_Y,Head_Z," <<
//                        "BTGripper_X,BTGripper_Y,BTGripper_Z," <<
                       "LPMS_QUAT_W,LPMS_QUAT_X,LPMS_QUAT_Y,LPMS_QUAT_Z," <<
                       "LPMS_ACCEL_RAW_X,LPMS_ACCEL_RAW_Y,LPMS_ACCEL_RAW_Z,"<<
                       "LPMS_GYRO_RAW_X,LPMS_GYRO_RAW_Y,LPMS_GYRO_RAW_Z," <<
                       "LPMS_MAT_11,LPMS_MAT_12,LPMS_MAT_13,LPMS_MAT_21,LPMS_MAT_22,LPMS_MAT_23,LPMS_MAT_31,LPMS_MAT_32,LPMS_MAT_33," <<
                       "LPMS_MAT_OFFSET_11,LPMS_MAT_OFFSET_12,LPMS_MAT_OFFSET_13,LPMS_MAT_OFFSET_21,LPMS_MAT_OFFSET_22,LPMS_MAT_OFFSET_23," <<
                       "LPMS_MAT_OFFSET_31,LPMS_MAT_OFFSET_32,LPMS_MAT_OFFSET_33\n";


            }
            else
            {
                recording=false;
                std::cout << "Stop Recording " << std::endl;
                myfile.close();
            }
        }

        if(recording==true)
        {
            myfile << *RShoulder_X << "," << *RShoulder_Y << "," << *RShoulder_Z << "," << *RElbow_X << "," << *RElbow_Y << "," << *RElbow_Z  << "," << *RHand_X << "," << *RHand_Y << "," << *RHand_Z << "," ;
            myfile << *LShoulder_X << "," << *LShoulder_Y << "," << *LShoulder_Z << "," << *LElbow_X << "," << *LElbow_Y << "," << *LElbow_Z  << "," << *LHand_X << "," << *LHand_Y << "," << *LHand_Z << "," ;
            myfile << *Torso_X << "," << *Torso_Y << "," << *Torso_Z << "," << *Head_X << "," << *Head_Y << "," << *Head_Z << "," ;
//             myfile << *BTGripper_X << "," << *BTGripper_Y << "," << *BTGripper_Z << "," ;
            myfile << *LPMS_QUAT_W << "," << *LPMS_QUAT_X << "," << *LPMS_QUAT_Y << "," << *LPMS_QUAT_Z << "," ;
            myfile << *LPMS_ACCEL_RAW_X << "," << *LPMS_ACCEL_RAW_Y << "," << *LPMS_ACCEL_RAW_Z << "," ;
            myfile << *LPMS_GYRO_RAW_X << "," << *LPMS_GYRO_RAW_Y << "," << *LPMS_GYRO_RAW_Z << "," ;
            myfile << *LPMS_MAT_11 << "," << *LPMS_MAT_12 << "," << *LPMS_MAT_13 << "," ;
            myfile << *LPMS_MAT_21 << "," << *LPMS_MAT_22 << "," << *LPMS_MAT_23 << "," ;
            myfile << *LPMS_MAT_31 << "," << *LPMS_MAT_32 << "," << *LPMS_MAT_33 << "," ;
            myfile << *LPMS_MAT_OFFSET_11 << "," << *LPMS_MAT_OFFSET_12 << "," << *LPMS_MAT_OFFSET_13 << "," ;
            myfile << *LPMS_MAT_OFFSET_21 << "," << *LPMS_MAT_OFFSET_22 << "," << *LPMS_MAT_OFFSET_23 << "," ;
            myfile << *LPMS_MAT_OFFSET_31 << "," << *LPMS_MAT_OFFSET_32 << "," << *LPMS_MAT_OFFSET_33 << "\n";
        }

        //Shared_Memory::getInstance().Print_Double_Values();
        usleep(500);
    }
    saveConfiguration();
    myfile.close();
    stoped=true;
    return;
}


bool File_Writer::loadConfiguration()
{
    std::stringstream path;
    path <<"../conf/" << PLUGIN_NAME;
    ConfigFile config( path.str() );
    return true;
}

bool File_Writer::saveConfiguration()
{
    std::stringstream path;
    path <<"../conf/" << PLUGIN_NAME;
    ConfigFile config( path.str() );
    std::ofstream file(path.str());
	file << config;
	file.close();
    return true;
}

bool File_Writer::Initialize_Output()
{
    return true;
}

bool File_Writer::Initialize_Input()
{

    RShoulder_X=Shared_Memory::getInstance().Register_Double_Input("RIGHT_SHOULDER_X");
    RShoulder_Y=Shared_Memory::getInstance().Register_Double_Input("RIGHT_SHOULDER_Y");
    RShoulder_Z=Shared_Memory::getInstance().Register_Double_Input("RIGHT_SHOULDER_Z");
    RElbow_X=Shared_Memory::getInstance().Register_Double_Input("RIGHT_ELBOW_X");
    RElbow_Y=Shared_Memory::getInstance().Register_Double_Input("RIGHT_ELBOW_Y");
    RElbow_Z=Shared_Memory::getInstance().Register_Double_Input("RIGHT_ELBOW_Z");
    RHand_X=Shared_Memory::getInstance().Register_Double_Input("RIGHT_HAND_X");
    RHand_Y=Shared_Memory::getInstance().Register_Double_Input("RIGHT_HAND_Y");
    RHand_Z=Shared_Memory::getInstance().Register_Double_Input("RIGHT_HAND_Z");

    LShoulder_X=Shared_Memory::getInstance().Register_Double_Input("LEFT_SHOULDER_X");
    LShoulder_Y=Shared_Memory::getInstance().Register_Double_Input("LEFT_SHOULDER_Y");
    LShoulder_Z=Shared_Memory::getInstance().Register_Double_Input("LEFT_SHOULDER_Z");
    LElbow_X=Shared_Memory::getInstance().Register_Double_Input("LEFT_ELBOW_X");
    LElbow_Y=Shared_Memory::getInstance().Register_Double_Input("LEFT_ELBOW_Y");
    LElbow_Z=Shared_Memory::getInstance().Register_Double_Input("LEFT_ELBOW_Z");
    LHand_X=Shared_Memory::getInstance().Register_Double_Input("LEFT_HAND_X");
    LHand_Y=Shared_Memory::getInstance().Register_Double_Input("LEFT_HAND_Y");
    LHand_Z=Shared_Memory::getInstance().Register_Double_Input("LEFT_HAND_Z");

    Torso_X=Shared_Memory::getInstance().Register_Double_Input("TORSO_X");
    Torso_Y=Shared_Memory::getInstance().Register_Double_Input("TORSO_Y");
    Torso_Z=Shared_Memory::getInstance().Register_Double_Input("TORSO_Z");

    Head_X=Shared_Memory::getInstance().Register_Double_Input("HEAD_X");
    Head_Y=Shared_Memory::getInstance().Register_Double_Input("HEAD_Y");
    Head_Z=Shared_Memory::getInstance().Register_Double_Input("HEAD_Z");
/*
    BTGripper_X=Shared_Memory::getInstance().Register_Double_Input("BTGripper_X");
    BTGripper_Y=Shared_Memory::getInstance().Register_Double_Input("BTGripper_Y");
    BTGripper_Z=Shared_Memory::getInstance().Register_Double_Input("BTGripper_Z");*/

    LPMS_QUAT_W=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_W");
    LPMS_QUAT_X=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_X");
    LPMS_QUAT_Y=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_Y");
    LPMS_QUAT_Z=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_Z");


    LPMS_QUAT_W=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_W");
    LPMS_QUAT_X=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_X");
    LPMS_QUAT_Y=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_Y");
    LPMS_QUAT_Z=Shared_Memory::getInstance().Register_Double_Input("LPMS_QUAT_Z");

    LPMS_ACCEL_RAW_X=Shared_Memory::getInstance().Register_Double_Input("LPMS_ACCEL_RAW_X");
    LPMS_ACCEL_RAW_Y=Shared_Memory::getInstance().Register_Double_Input("LPMS_ACCEL_RAW_Y");
    LPMS_ACCEL_RAW_Z=Shared_Memory::getInstance().Register_Double_Input("LPMS_ACCEL_RAW_Z");

    LPMS_GYRO_RAW_X=Shared_Memory::getInstance().Register_Double_Input("LPMS_GYRO_RAW_X");
    LPMS_GYRO_RAW_Y=Shared_Memory::getInstance().Register_Double_Input("LPMS_GYRO_RAW_Y");
    LPMS_GYRO_RAW_Z=Shared_Memory::getInstance().Register_Double_Input("LPMS_GYRO_RAW_Z");

    LPMS_MAT_11=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_11");
    LPMS_MAT_12=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_12");
    LPMS_MAT_13=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_13");
    LPMS_MAT_21=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_21");
    LPMS_MAT_22=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_22");
    LPMS_MAT_23=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_23");
    LPMS_MAT_31=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_31");
    LPMS_MAT_32=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_32");
    LPMS_MAT_33=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_33");

    LPMS_MAT_OFFSET_11=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_11");
    LPMS_MAT_OFFSET_12=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_12");
    LPMS_MAT_OFFSET_13=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_13");
    LPMS_MAT_OFFSET_21=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_21");
    LPMS_MAT_OFFSET_22=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_22");
    LPMS_MAT_OFFSET_23=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_23");
    LPMS_MAT_OFFSET_31=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_31");
    LPMS_MAT_OFFSET_32=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_32");
    LPMS_MAT_OFFSET_33=Shared_Memory::getInstance().Register_Double_Input("LPMS_MAT_OFFSET_33");

    return true;
}

void File_Writer::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
    running = true;
	stoped = false;
}

void File_Writer::stop()
{
    running = false;
}


