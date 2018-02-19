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

#include "GL_Window.h"
#include "tab.h"
#include "input_singleton.h"
#include "view2d.h"

#include <math.h>
#define PI 3.14159

GL_Window::GL_Window(int x, int y, int widht, int height, std::string name, Tab* parent)
{

    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    this->SetActive(true);
    parent->InsertWidget(this);
}


////////////////////////////// CREATE FROM AXIS ANGLE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function converts an axis and angle rotation to a quaternion
/////
////////////////////////////// CREATE FROM AXIS ANGLE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CQuaternion::CreateFromAxisAngle(float X, float Y, float Z, float degree)
{
    // This function takes an angle and an axis of rotation, then converts
    // it to a quaternion.  An example of an axis and angle is what we pass into
    // glRotatef().  That is an axis angle rotation.  It is assumed an angle in
    // degrees is being passed in.  Instead of using glRotatef(), we can now handle
    // the rotations our self.

    // The equations for axis angle to quaternions are such:

    // w = cos( theta / 2 )
    // x = X * sin( theta / 2 )
    // y = Y * sin( theta / 2 )
    // z = Z * sin( theta / 2 )

    // First we want to convert the degrees to radians
    // since the angle is assumed to be in radians
    float angle = float((degree / 180.0f) * PI);

    // Here we calculate the sin( theta / 2) once for optimization
    float result = (float)sin( angle / 2.0f );

    // Calcualte the w value by cos( theta / 2 )
    w = (float)cos( angle / 2.0f );

    // Calculate the x, y and z of the quaternion
    x = float(X * result);
    y = float(Y * result);
    z = float(Z * result);
}


////////////////////////////// CREATE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function converts a quaternion to a rotation matrix
/////
////////////////////////////// CREATE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CQuaternion::CreateMatrix(float *pMatrix)
{
    // Make sure the matrix has allocated memory to store the rotation data
    if(!pMatrix) return;

    // This function is a necessity when it comes to doing almost anything
    // with quaternions.  Since we are working with OpenGL, which uses a 4x4
    // homogeneous matrix, we need to have a way to take our quaternion and
    // convert it to a rotation matrix to modify the current model view matrix.
    // We pass in a 4x4 matrix, which is a 1D array of 16 floats.  This is how OpenGL
    // allows us to pass in a matrix to glMultMatrixf(), so we use a single dimensioned array.
    // After about 300 trees murdered and 20 packs of chalk depleted, the
    // mathematicians came up with these equations for a quaternion to matrix converion:
    //
    //     Š        2     2												 Š
    //     Š 1 - (2y  + 2z )   2xy + 2zw         2xz - 2yw			0	 Š
    //     Š															 Š
    //     Š                          2     2							 Š
    // M = Š 2xy - 2zw         1 - (2x  + 2z )   2zy + 2xw			0	 Š
    //     Š															 Š
    //     Š                                            2     2			 Š
    //     Š 2xz + 2yw         2yz - 2xw         1 - (2x  + 2y )	0	 Š
    //     Š															 Š
    //     Š															 Š
    //     Š 0				   0				 0					1	 |													 Š
    //     Š															 Š
    //
    // This is of course a 4x4 matrix.  Notice that a rotational matrix can just
    // be a 3x3 matrix, but since OpenGL uses a 4x4 matrix, we need to conform to the man.
    // Remember that the identity matrix of a 4x4 matrix has a diagonal of 1's, where
    // the rest of the indices are 0.  That is where we get the 0's lining the sides, and
    // the 1 at the bottom-right corner.  Since OpenGL matrices are row by column, we fill
    // in our matrix accordingly below.

    // First row
    pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );
    pMatrix[ 1] = 2.0f * (x * y + z * w);
    pMatrix[ 2] = 2.0f * (x * z - y * w);
    pMatrix[ 3] = 0.0f;

    // Second row
    pMatrix[ 4] = 2.0f * ( x * y - z * w );
    pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );
    pMatrix[ 6] = 2.0f * (z * y + x * w );
    pMatrix[ 7] = 0.0f;

    // Third row
    pMatrix[ 8] = 2.0f * ( x * z + y * w );
    pMatrix[ 9] = 2.0f * ( y * z - x * w );
    pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );
    pMatrix[11] = 0.0f;

    // Fourth row
    pMatrix[12] = 0;
    pMatrix[13] = 0;
    pMatrix[14] = 0;
    pMatrix[15] = 1.0f;

    // Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}




void GL_Window::Draw()
{

    resetPerspectiveProjection();
    glPushMatrix();
    gluLookAt(0, 0, 2,  0, 0, 0,   0, 1, 0);

    // Create a 4x4 matrix to be used to convert a quaternion to a matrix
    float matrix[16];

    quat.CreateMatrix(matrix);

    // To apply our rotation, we multiply the new quaternion matrix by the current matrix.
    glColor3f(1,0,0);
    glutWireSphere(.05,4,4);
    glColor3f(1,1,1);
    glMultMatrixf(matrix);
    glutWireCone(.05,.5,4,10);
    glTranslatef(0,0,.5);
    glColor3f(1,0,0);
    glutWireSphere(.01,4,4);
    glPopMatrix();

    setOrthographicProjection ( 1024, 768);
}

void GL_Window::update()
{

}



void GL_Window::proccesInput()
{


}