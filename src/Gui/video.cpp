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
#include "../Logger_API/easylogging++.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "video.h"
#include "tab.h"

pthread_mutex_t video_lock;
GLuint texture;

Video::Video(int x, int y, int widht, int height, std::string name, Tab* parent)
{
    if (pthread_mutex_init(&video_lock, NULL) != 0)
    {
        LOG(WARNING) << "Video mutex "<< name << " lock init failed";
        return;
    }
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    image=NULL;
    parent->InsertWidget(this);
    
    
}

void Video::update()
{

}

void Video::SetImage (IplImage * value)
{
    pthread_mutex_lock(&video_lock);
    cvReleaseImage(&image);
    image = cvCloneImage(value);
    pthread_mutex_unlock(&video_lock);
}

void DrawIplImage(IplImage *image, int x = 0, int y = 0, GLfloat xZoom = 1.0f, GLfloat yZoom = 1.0f)
{
    GLenum format;
    switch(image->nChannels) {
        case 1:
            format = GL_LUMINANCE;
            break;
        case 2:
            format = GL_LUMINANCE_ALPHA;
            break;
        case 3:
            format = GL_BGR;
            break;
        default:
            return;
    }
    
    glRasterPos2i(x, y);
    glPixelZoom(xZoom, -yZoom);
    glDrawPixels(image->width, image->height, format, GL_UNSIGNED_BYTE, image->imageData);
    glPixelZoom(xZoom, yZoom);
}

void Video::Draw()
{
    pthread_mutex_lock(&video_lock);
    if (image!=NULL)
    DrawIplImage(image, Get_x(), Get_y());
    pthread_mutex_unlock(&video_lock);
}

void Video::proccesInput()
{

}

