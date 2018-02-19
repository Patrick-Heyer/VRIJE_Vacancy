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


#ifndef NI_DEPTH
#define NI_DEPTH

#pragma once

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>

#include "widget.h"


class Ni_Depth : public Widget
{
public:
        Ni_Depth(int x, int y, xn::DepthMetaData * DepthData, xn::UserGenerator g_UserGenerator, xn::DepthGenerator g_DepthGenerator, std::string name, Tab* parent);
        void Draw();
        void proccesInput();
        void update();

        
private:
     xn::DepthMetaData *dmd;
     xn::UserGenerator UserGenerator;
     xn::DepthGenerator DepthGenerator;

};



#endif // NI_DEPTH
