/*
*    <Color_Tracker sends data to Hocoma games from this architecture.>
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



#include <opencv/cv.hpp>
#include <opencv/highgui.h>

#include "Color_Tracker.h"

INITIALIZE_NULL_EASYLOGGINGPP

cv::Mat *frame_input;
cv::Mat frame;
cv::Mat edges, HSV_image, HSV_image_local;
cv::Scalar hsvlow(0,0,200),hsvhigh(255,255,255);

void  getHSV(int event, int x, int y, int, void* );
int polo=0;

void Color_Tracker::Main()
{
    IplImage imgThresh;


	while (running)
    {
		if (frame_input->data != NULL)
		{
			frame_input->copyTo(frame);


			cv::flip(frame, frame, 1);


			cv::cvtColor(frame, HSV_image, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
			cv::inRange(HSV_image, hsvlow, hsvhigh, HSV_image); //Threshold the image

			//morphological opening (removes small objects from the foreground)
			cv::erode(HSV_image, HSV_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
			cv::dilate(HSV_image, HSV_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			cv::dilate(HSV_image, HSV_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
			cv::erode(HSV_image, HSV_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

			imgThresh = HSV_image;
			CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
			cvMoments(&imgThresh, moments, 1);
            double moment00 = cvGetSpatialMoment(moments, 0, 0);
			double moment10 = cvGetSpatialMoment(moments, 1, 0);
			double moment01 = cvGetSpatialMoment(moments, 0, 1);
			double area = cvGetCentralMoment(moments, 0, 0);
			cv::RNG rng(12345);
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			// if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero


			cv::Point center;
            center.x = (int)(moment10/moment00); 
            center.y = (int)(moment01/moment00); 
            
            posX=(double)((moment10/moment00)/300)-1;
            posY=(double)((moment01/moment00)/200)-1;
			

			cv::circle(frame,center, sqrt(area*.5), color, 2, 8, 0);


			free(moments);

			cv::imshow("Thresholded Image", HSV_image); //show the thresholded image
            cv::namedWindow( "edges", CV_WINDOW_AUTOSIZE );
			cv::imshow("edges", frame);

			cv::setMouseCallback("edges", getHSV, 0);
			cv::waitKey(100);
          
    
		}
    }
   

	stoped = true;
    return;
}

void  getHSV(int event, int x, int y, int, void* ) {

    int loop;
    float change;

    if( event == cv::EVENT_LBUTTONDOWN ) {
        cv::cvtColor(frame, HSV_image, CV_BGR2HSV);
        cv::Vec3b p = HSV_image.at<cv::Vec3b>(y,x);
        for (loop=0; loop<3; loop++) {
            change=256*.25;
            hsvlow[loop]  = p[loop] - change;
            hsvhigh[loop] = p[loop] + change;
            std::cout << "hsvlow" << loop << "  " <<hsvlow[loop] << std::endl;
            std::cout << "hsvhigh" << loop << "  "<< hsvhigh[loop] << std::endl;
        }
    }
}

bool Color_Tracker::loadConfiguration()
{
	return true;
}

bool Color_Tracker::saveConfiguration()
{
	return true;
}

bool Color_Tracker::Initialize_Output()
{
    Shared_Memory::getInstance().Register_Output("Tracker_X", &posX);
    Shared_Memory::getInstance().Register_Output("Tracker_Y", &posY);
	return true;
}

bool Color_Tracker::Initialize_Input()
{
	frame_input = static_cast<cv::Mat*>(Shared_Memory::getInstance().Register_Void_Input("Video_From_Camera"));
	return true;
}
void Color_Tracker::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
	running = true;
	stoped = false;
}

void Color_Tracker::stop()
{
	running = false;

}
