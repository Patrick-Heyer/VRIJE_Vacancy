/*
 * < *one line to* give the program's name and a brief idea of what it does.>
 * Copyright (C) 2011  <Patrick Heyer> <patrickhey@prodigy.net.mx>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/glut.h>

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>

#include <map>

#include "Ni_Depth.h"
#include "tab.h"


#define MAX_DEPTH 10000
float g_pDepthHist[MAX_DEPTH];

GLfloat texcoords[8];
XnUInt32 nColors = 10;


std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > m_Errors;

unsigned int getClosestPowerOfTwo(unsigned int n)
{
        unsigned int m = 2;
        while(m < n) m<<=1;

        return m;
}

void glPrintString(void *font, char *str)
{
        int i,l = strlen(str);

        for(i=0; i<l; i++)
        {
                glutBitmapCharacter(font,*str++);
        }
}

XnFloat Colors[][3] =
{
        {0,1,1},
        {0,0,1},
        {0,1,0},
        {1,1,0},
        {1,0,0},
        {1,.5,0},
        {.5,1,0},
        {0,.5,1},
        {.5,0,1},
        {1,1,.5},
        {1,1,1}
};

const XnChar* GetPoseErrorString(XnPoseDetectionStatus error)
{
        switch (error)
        {
        case XN_POSE_DETECTION_STATUS_OK:
                return "OK";
        case XN_POSE_DETECTION_STATUS_NO_USER:
                return "NoUser";
        case XN_POSE_DETECTION_STATUS_TOP_FOV:
                return "Top FOV";
        case XN_POSE_DETECTION_STATUS_SIDE_FOV:
                return "Side FOV";
        case XN_POSE_DETECTION_STATUS_ERROR:
                return "General error";
        default:
                return "Unknown";
        }
}

const XnChar* GetCalibrationErrorString(XnCalibrationStatus error)
{
        switch (error)
        {
        case XN_CALIBRATION_STATUS_OK:
                return "OK";
        case XN_CALIBRATION_STATUS_NO_USER:
                return "NoUser";
        case XN_CALIBRATION_STATUS_ARM:
                return "Arm";
        case XN_CALIBRATION_STATUS_LEG:
                return "Leg";
        case XN_CALIBRATION_STATUS_HEAD:
                return "Head";
        case XN_CALIBRATION_STATUS_TORSO:
                return "Torso";
        case XN_CALIBRATION_STATUS_TOP_FOV:
                return "Top FOV";
        case XN_CALIBRATION_STATUS_SIDE_FOV:
                return "Side FOV";
        case XN_CALIBRATION_STATUS_POSE:
                return "Pose";
        default:
                return "Unknown";
        }
}

GLuint initTexture(void** buf, int& width, int& height)
{
        GLuint texID = 0;
        glGenTextures(1,&texID);

        width = getClosestPowerOfTwo(width);
        height = getClosestPowerOfTwo(height); 
        *buf = new unsigned char[width*height*4];
        glBindTexture(GL_TEXTURE_2D,texID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texID;
}

void DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
        GLfloat verts[8] = {    topLeftX, topLeftY,
                topLeftX, bottomRightY,
                bottomRightX, bottomRightY,
                bottomRightX, topLeftY
        };
        glVertexPointer(2, GL_FLOAT, 0, verts);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //TODO: Maybe glFinish needed here instead - if there's some bad graphics crap
        glFlush();
}

void DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

        DrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

Ni_Depth::Ni_Depth(int x, int y, xn::DepthMetaData * DepthData, xn::UserGenerator g_UserGenerator, xn::DepthGenerator g_DepthGenerator, std::string name, Tab* parent)
{
     this->SetName(name);
     this->SetPosition(x, y);
     this->UserGenerator=g_UserGenerator;
     this->dmd=DepthData;
     parent->InsertWidget(this);
}

void Ni_Depth::Draw()
{
    static bool bInitialized = false;   
        static GLuint depthTexID;
        static unsigned char* pDepthTexBuf;
        static int texWidth, texHeight;

        float topLeftX;
        float topLeftY;
        float bottomRightY;
        float bottomRightX;
        float texXpos;
        float texYpos;
        int userCount;

        userCount = 0;
        if(!bInitialized)
        {
                texWidth =  getClosestPowerOfTwo(dmd->XRes());
                texHeight = getClosestPowerOfTwo(dmd->YRes());

//              printf("Initializing depth texture: width = %d, height = %d\n", texWidth, texHeight);
                depthTexID = initTexture((void**)&pDepthTexBuf,texWidth, texHeight) ;

//              printf("Initialized depth texture: width = %d, height = %d\n", texWidth, texHeight);
                bInitialized = true;

                topLeftX = dmd->XRes();
                topLeftY = 0;
                bottomRightY = dmd->YRes();
                bottomRightX = 0;
                texXpos =(float)dmd->XRes()/texWidth;
                texYpos  =(float)dmd->YRes()/texHeight;

                memset(texcoords, 0, 8*sizeof(float));
                texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;
        }

        unsigned int nValue = 0;
        unsigned int nHistValue = 0;
        unsigned int nIndex = 0;
        unsigned int nX = 0;
        unsigned int nY = 0;
        unsigned int nNumberOfPoints = 0;
        XnUInt16 g_nXRes = dmd->XRes();
        XnUInt16 g_nYRes = dmd->YRes();

        unsigned char* pDestImage = pDepthTexBuf;

        const XnDepthPixel* pDepth = dmd->Data();
//         const XnLabel* pLabels = smd.Data();

        // Calculate the accumulative histogram
        memset(g_pDepthHist, 0, MAX_DEPTH*sizeof(float));
        for (nY=0; nY<g_nYRes; nY++)
        {
                for (nX=0; nX<g_nXRes; nX++)
                {
                        nValue = *pDepth;

                        if (nValue != 0)
                        {
                                g_pDepthHist[nValue]++;
                                nNumberOfPoints++;
                        }

                        pDepth++;
                }
        }

        for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
        {
                g_pDepthHist[nIndex] += g_pDepthHist[nIndex-1];
        }
        if (nNumberOfPoints)
        {
                for (nIndex=1; nIndex<MAX_DEPTH; nIndex++)
                {
                        g_pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (g_pDepthHist[nIndex] / nNumberOfPoints)));
                }
        }

        pDepth = dmd->Data();
        if (1 /*g_bDrawPixels*/)
        {
                XnUInt32 nIndex = 0;
                // Prepare the texture map
                for (nY=0; nY<g_nYRes; nY++)
                {
                        for (nX=0; nX < g_nXRes; nX++, nIndex++)
                        {

                                pDestImage[0] = 0;
                                pDestImage[1] = 0;
                                pDestImage[2] = 0;
//                                 if (g_bDrawBackground || *pLabels != 0)
//                                 {
//                                         nValue = *pDepth;
//                                         XnLabel label = *pLabels;
//                                         XnUInt32 nColorID = label % nColors;
//                                         if (label == 0)
//                                         {
//                                                 nColorID = nColors;
//                                         }
// 
//                                         if (nValue != 0)
//                                         {
//                                                 nHistValue = g_pDepthHist[nValue];
// 
//                                                 pDestImage[0] = nHistValue * Colors[nColorID][0]; 
//                                                 pDestImage[1] = nHistValue * Colors[nColorID][1];
//                                                 pDestImage[2] = nHistValue * Colors[nColorID][2];
//                                         }
//                                 }

                                pDepth++;
//                                 pLabels++;
                                pDestImage+=3;
                        }

                        pDestImage += (texWidth - g_nXRes) *3;
                }
        }
        else
        {
                xnOSMemSet(pDepthTexBuf, 0, 3*2*g_nXRes*g_nYRes);
        }

        glBindTexture(GL_TEXTURE_2D, depthTexID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pDepthTexBuf);

        // Display the OpenGL texture map
        glColor4f(0.75,0.75,0.75,1);

        glEnable(GL_TEXTURE_2D);
        DrawTexture(dmd->XRes(),dmd->YRes(),0,0); 
        glDisable(GL_TEXTURE_2D);

        char strLabel[50] = "";
        XnUserID aUsers[15];
        XnUInt16 nUsers = 15;
        this->UserGenerator.GetUsers(aUsers, nUsers);
        for (int i = 0; i < nUsers; ++i)
        {
/*
                if (g_bPrintID)
                {
                        XnPoint3D com;
                        this->UserGenerator.GetCoM(aUsers[i], com);
                        this->DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

                        xnOSMemSet(strLabel, 0, sizeof(strLabel));
                        if (!g_bPrintState)
                        {
                                // Tracking
                                sprintf(strLabel, "%d", aUsers[i]);
                        }
                        else if (this->UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
                        {
                                // Tracking
                                sprintf(strLabel, "%d - Tracking", aUsers[i]);
                        }
                        else if (this->UserGenerator.GetSkeletonCap().IsCalibrating(aUsers[i]))
                        {
                                // Calibrating
                                sprintf(strLabel, "%d - Calibrating [%s]", aUsers[i], GetCalibrationErrorString(m_Errors[aUsers[i]].first));
                        }
                        else
                        {
                                // Nothing
                                sprintf(strLabel, "%d - Looking for pose [%s]", aUsers[i], GetPoseErrorString(m_Errors[aUsers[i]].second));
                        }


                        glColor4f(1-Colors[i%nColors][0], 1-Colors[i%nColors][1], 1-Colors[i%nColors][2], 1);

                        glRasterPos2i(com.X, com.Y);
                        glPrintString(GLUT_BITMAP_HELVETICA_18, strLabel);
                }*/

               
        }

}

void Ni_Depth::proccesInput()
{

}

void Ni_Depth::update()
{

}
