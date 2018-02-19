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


#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#pragma once

#include <list>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <sstream>



#include "view2d.h"
#include "widget.h"

// This is our basic vector class
struct CVector3
{
public:
	
	// A default constructor
	CVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// The x, y and z values of this vector
	float x, y, z;						
};


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is our basic quaternion class
class CQuaternion 
{

public:

	// This is our default constructor, which initializes everything to an identity
	// quaternion.  An identity quaternion has x, y, z as 0 and w as 1.
	CQuaternion() { x = y = z = 0.0f;   w = 1.0f;  }
	
	// Create a constructor that will allow us to initialize the quaternion when creating it
	CQuaternion(float X, float Y, float Z, float W) 
	{ 
		x = X;		y = Y;		z = Z;		w = W;
	}

	// This converts and axis angle rotation to a quaternion
	void CreateFromAxisAngle(float X, float Y, float Z, float degree);

	// This takes in an array of 16 floats to fill in a 4x4 homogeneous matrix from a quaternion
	void CreateMatrix(float *pMatrix);
        void mat_to_quat(float *pMatrix);


	// This stores the 4D values for the quaternion
	float x, y, z, w;
};



class GL_Window : public Widget
{
public:
    GL_Window(int x, int y, int widht, int height, std::string name, Tab* parent);
    void Draw();
    void update ();
    void proccesInput();
    

    CQuaternion quat;
};



#endif // CONSOLE_H
