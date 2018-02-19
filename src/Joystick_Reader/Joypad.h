//----------------------------------------------------------------------------------------
/// \file	Joypad.h
/// \author	Rob Bateman
/// \date	12-jan-2004
/// \brief	This file provides a generic cross platform joypad interface for Win32 &
/// 		Linux. Generally the linux implimentation is not as cool as using DirectX,
/// 		you only really get hold of the values. Using force feedback is beyond Linux
/// 		at the moment. Oh well...
///
//----------------------------------------------------------------------------------------



#ifndef __JOYPAD__H__
#define __JOYPAD__H__

#ifdef WIN32

	#pragma once

	// link the DirectX libraries
	#pragma comment(lib,"dxguid.lib")
	//#pragma comment(lib,"dinput.lib")
	#pragma comment(lib,"dinput8.lib")

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	
	#define DIRECTINPUT_VERSION 0x0800
	#include <dinput.h>

#endif

#include <string>
#include <iostream>




/// make sure DWORD and LONG and defined in linux land
#ifndef WIN32
typedef long LONG;
typedef unsigned long DWORD;
#endif


//-----------------------------------------------------------------
///	\brief	
///
struct XInputJoypad 
{	
	
	//////////////////////////////////////////////////////////////////////////	INTERNAL STUPH

	/// an enumeration to represent the various axes that may be available
	enum eAxes
	{
		X_AXIS		= 0x00000001,
		Y_AXIS		= 0x00000002,
		Z_AXIS		= 0x00000004,

		X_ROT_AXIS	= 0x00000008,
		Y_ROT_AXIS  = 0x00000010,
		Z_ROT_AXIS  = 0x00000020,

		SLIDER_1    = 0x00000040,
		SLIDER_2    = 0x00000080,

		// force 32bit value
		AXIS_LAST	= 0xFFFFFFFF
	};

	virtual bool Update() = 0;

	////////////////////////////////////////////////////////////////////////// DEVICE INFO
	virtual const char* const InstanceName() const = 0;
	virtual const char* const ProductName() const = 0;



	////////////////////////////////////////////////////////////////////////// DEVICE CAPABILITIES

	///
	/// this function returns the number of buttons available on the joypad
	///
	virtual const unsigned short NumButtons() const = 0;

	///
	/// this function returns the number of buttons available on the joypad
	///
	virtual const unsigned short NumPovs() const = 0;


	///
	/// the following functions allow you to check to see if the specified
	/// axis is available on the joypad.
	///
	virtual bool HasX() const= 0;
	virtual bool HasY() const = 0;
	virtual bool HasZ() const = 0;
	virtual bool HasXrot() const = 0;
	virtual bool HasYrot() const = 0;
	virtual bool HasZrot() const = 0;
	virtual bool HasExtra1() const = 0;
	virtual bool HasExtra2() const = 0;


	//////////////////////////////////////////////////////////////////////////	BUTTONS


	/// Array of buttons. The high-order bit of the byte is set if the
	/// corresponding button is down, and clear if the button is up or does
	/// not exist.
	///
	virtual const bool Button(const unsigned int& i) const = 0;



	//////////////////////////////////////////////////////////////////////////	X AXIS

	///	X-axis, usually the left-right movement of a stick.
	virtual const LONG Xaxis() const = 0;

	/// X-axis velocity.
	virtual const LONG Xvelocity() const = 0;

	/// X-axis acceleration.
	virtual const LONG Xacceleration() const = 0;

	/// X-axis force.
	virtual const LONG Xforce() const = 0;


	//////////////////////////////////////////////////////////////////////////	Y AXIS



	///	Y-axis, usually the forward-backward movement of a stick.
	virtual const LONG Yaxis() const = 0;

	/// Y-axis velocity.
	virtual const LONG Yvelocity() const = 0;

	/// Y-axis acceleration.
	virtual const LONG Yacceleration() const = 0;

	/// Y-axis force.
	virtual const LONG Yforce() const = 0;



	//////////////////////////////////////////////////////////////////////////	Z AXIS

	///	Z-axis, often the throttle control. If the joystick does not have this
	/// axis, the value is 0.
	virtual const LONG Zaxis() const = 0;

	/// Z-axis velocity.
	virtual const LONG Zvelocity() const = 0;

	/// Z-axis acceleration.
	virtual const LONG Zacceleration() const = 0;

	/// Z-axis force.
	virtual const LONG Zforce() const = 0;




	//////////////////////////////////////////////////////////////////////////	X AXIS ROTATION

	///	X-axis rotation. If the joystick does not have this axis, the value is 0.
	virtual const LONG Xrot() const = 0;

	/// X-axis angular velocity.
	virtual const LONG XrotVelocity() const = 0;

	/// X-axis angular acceleration.
	virtual const LONG XrotAcceleration() const = 0;

	/// X-axis torque.
	virtual const LONG XrotForce() const = 0;



	//////////////////////////////////////////////////////////////////////////	Y AXIS ROTATION

	///	Y-axis rotation. If the joystick does not have this axis, the value is 0.
	virtual const LONG Yrot() const = 0;

	/// Y-axis angular velocity.
	virtual const LONG YrotVelocity() const = 0;

	/// Y-axis angular acceleration.
	virtual const LONG YrotAcceleration() const = 0;

	/// Y-axis torque.
	virtual const LONG YrotForce() const = 0;




	//////////////////////////////////////////////////////////////////////////	Z AXIS ROTATION

	///	Z-axis rotation (often called the rudder). If the joystick does not have
	/// this axis, the value is 0.
	virtual const LONG Zrot() const = 0;

	/// Z-axis angular velocity. .
	virtual const LONG ZrotVelocity() const = 0;

	/// Z-axis angular acceleration.
	virtual const LONG ZrotAcceleration() const = 0;

	/// Z-axis torque.
	virtual const LONG ZrotForce() const = 0;



	//////////////////////////////////////////////////////////////////////////	2 ADDITIONAL AXES

	///	Two additional axis values (formerly called the u-axis and v-axis) whose
	/// semantics depend on the joystick. Use the IDirectInputDevice8::GetObjectInfo
	/// method to obtain semantic information about these values.
	virtual const LONG ExtraAxes(const unsigned int& i) const = 0;

	//Extra axis velocities.
	virtual const LONG ExtraVelocities(const unsigned int& i) const = 0;

	//Extra axis accelerations.
	virtual const LONG ExtraAccelerations(const unsigned int& i) const = 0;

	//Extra axis forces.
	virtual const LONG ExtraForces(const unsigned int& i) const = 0;


	//////////////////////////////////////////////////////////////////////////	POV

	/// Direction controllers, such as point-of-view hats. The position is
	/// indicated in hundredths of a degree clockwise from north (away from the
	/// user). The center position is normally reported as -1; but see Remarks.
	/// For indicators that have only five positions, the value for a controller
	/// is -1, 0, 9,000, 18,000, or 27,000.
	virtual const DWORD POV(const unsigned int& i) const = 0;

};



//----------------------------------------------------------------------------------------
/// \brief	This function returns the number of working joypads attached to the
///			computer. 
/// 
extern unsigned int NumJoypads();

//----------------------------------------------------------------------------------------
/// \brief	This function returns a pointer to the requested Joypad or NULL if
///			no joypad is available.
/// 
extern XInputJoypad* GetJoypad(const unsigned int=0);

//----------------------------------------------------------------------------------------
/// \brief	This function is called to destroy the input at the end of the game
///
extern void FreeInput();

//----------------------------------------------------------------------------------------
/// \brief	This function can be used to initialise the joypad lib
/// \return	true if OK, false if it fails
extern bool InitInput();

//----------------------------------------------------------------------------------------
/// \brief	You should call this to update the state of the joypad
/// \return	true if OK.
///
extern bool UpdateInputState();


#ifdef WIN32

//----------------------------------------------------------------------------------------
/// The WIN32 implimentation of the joypad interface class.
///
class CJoypad : public XInputJoypad
{
public:


	virtual bool Update();

	////////////////////////////////////////////////////////////////////////// DEVICE INFO

	const char* const InstanceName() const {
		return m_Instance.c_str();
	}
	const char* const ProductName() const {
		return m_Product.c_str();
	}

	////////////////////////////////////////////////////////////////////////// DEVICE CAPABILITIES

	///
	/// this function returns the number of buttons available on the joypad
	///
	const unsigned short NumButtons() const {
		return m_nButtons;
	}

	///
	/// this function returns the number of buttons available on the joypad
	///
	const unsigned short NumPovs() const {
		return m_nPovs;
	}

	///
	/// the following functions allow you to check to see if the specified
	/// axis is available on the joypad.
	///

	bool HasX() const {
		return ((m_iAxes & X_AXIS) != 0x00000000);
	}
	bool HasY() const {
		return ((m_iAxes & Y_AXIS) != 0x00000000);
	}
	bool HasZ() const {
		return ((m_iAxes & Z_AXIS) != 0x00000000);
	}
	bool HasXrot() const {
		return ((m_iAxes & X_ROT_AXIS) != 0x00000000);
	}
	bool HasYrot() const {
		return ((m_iAxes & Y_ROT_AXIS) != 0x00000000);
	}
	bool HasZrot() const {
		return ((m_iAxes & Z_ROT_AXIS) != 0x00000000);
	}
	bool HasExtra1() const {
		return ((m_iAxes & SLIDER_1) != 0x00000000);
	}
	bool HasExtra2() const {
		return ((m_iAxes & SLIDER_2) != 0x00000000);
	}

	//////////////////////////////////////////////////////////////////////////	BUTTONS


	/// Array of buttons. The high-order bit of the byte is set if the
	/// corresponding button is down, and clear if the button is up or does
	/// not exist.
	///
	const bool Button(const unsigned int& i) const {
		return m_js.rgbButtons[i] != 0;
	}


	//////////////////////////////////////////////////////////////////////////	X AXIS

	///	X-axis, usually the left-right movement of a stick.
	const LONG Xaxis() const {
		return m_js.lX;
	}
	/// X-axis velocity.
	const LONG Xvelocity() const {
		return m_js.lVX;
	}
	/// X-axis acceleration.
	const LONG Xacceleration() const {
		return m_js.lAX;
	}
	/// X-axis force.
	const LONG Xforce() const {
		return m_js.lFX;
	}


	//////////////////////////////////////////////////////////////////////////	Y AXIS



	///	Y-axis, usually the forward-backward movement of a stick.
	const LONG Yaxis() const {
		return m_js.lY;
	}
	/// Y-axis velocity.
	const LONG Yvelocity() const {
		return m_js.lVY;
	}
	/// Y-axis acceleration.
	const LONG Yacceleration() const {
		return m_js.lAY;
	}
	/// Y-axis force.
	const LONG Yforce() const {
		return m_js.lFY;
	}


	//////////////////////////////////////////////////////////////////////////	Z AXIS

	///	Z-axis, often the throttle control. If the joystick does not have this
	/// axis, the value is 0.
	const LONG Zaxis() const {
		return m_js.lZ;

	}
	/// Z-axis velocity.
	const LONG Zvelocity() const {
		return m_js.lVZ;
	}
	/// Z-axis acceleration.
	const LONG Zacceleration() const {
		return m_js.lAZ;
	}
	/// Z-axis force.
	const LONG Zforce() const {
		return m_js.lFZ;
	}



	//////////////////////////////////////////////////////////////////////////	X AXIS ROTATION

	///	X-axis rotation. If the joystick does not have this axis, the value is 0.
	const LONG Xrot() const {
		return m_js.lX;
	}
	/// X-axis angular velocity.
	const LONG XrotVelocity() const {
		return m_js.lVRx;
	}
	/// X-axis angular acceleration.
	const LONG XrotAcceleration() const {
		return m_js.lARx;
	}
	/// X-axis torque.
	const LONG XrotForce() const {
		return m_js.lFRx;
	}


	//////////////////////////////////////////////////////////////////////////	Y AXIS ROTATION

	///	Y-axis rotation. If the joystick does not have this axis, the value is 0.
	const LONG Yrot() const {
		return m_js.lX;
	}
	/// Y-axis angular velocity.
	const LONG YrotVelocity() const {
		return m_js.lVRy;
	}
	/// Y-axis angular acceleration.
	const LONG YrotAcceleration() const {
		return m_js.lARy;
	}
	/// Y-axis torque.
	const LONG YrotForce() const {
		return m_js.lFRy;
	}



	//////////////////////////////////////////////////////////////////////////	Z AXIS ROTATION

	///	Z-axis rotation (often called the rudder). If the joystick does not have
	/// this axis, the value is 0.
	const LONG Zrot() const {
		return m_js.lZ;
	}
	/// Z-axis angular velocity. .
	const LONG ZrotVelocity() const {
		return m_js.lVRz;
	}
	/// Z-axis angular acceleration.
	const LONG ZrotAcceleration() const {
		return m_js.lARz;
	}
	/// Z-axis torque.
	const LONG ZrotForce() const {
		return m_js.lFRz;
	}


	//////////////////////////////////////////////////////////////////////////	2 ADDITIONAL AXES

	///	Two additional axis values (formerly called the u-axis and v-axis) whose
	/// semantics depend on the joystick. Use the IDirectInputDevice8::GetObjectInfo
	/// method to obtain semantic information about these values.
	const LONG ExtraAxes(const unsigned int& i) const {
		return m_js.rglSlider[i];
	}
	//Extra axis velocities.
	const LONG ExtraVelocities(const unsigned int& i) const {
		return m_js.rglVSlider[i];
	}
	//Extra axis accelerations.
	const LONG ExtraAccelerations(const unsigned int& i) const {
		return m_js.rglASlider[i];
	}
	//Extra axis forces.
	const LONG ExtraForces(const unsigned int& i) const {
		return m_js.rglFSlider[i];
	}


	//////////////////////////////////////////////////////////////////////////	POV

	/// Direction controllers, such as point-of-view hats. The position is
	/// indicated in hundredths of a degree clockwise from north (away from the
	/// user). The center position is normally reported as -1; but see Remarks.
	/// For indicators that have only five positions, the value for a controller
	/// is -1, 0, 9,000, 18,000, or 27,000.
	const DWORD POV(const unsigned int& i) const {
		return m_js.rgdwPOV[i];
	}


	//////////////////////////////////////////////////////////////////////////	Remarks
	/// 
	/// You must prepare the device for access to a joystick with extended 
	/// capabilities by calling the IDirectInputDevice8::SetDataFormat method, 
	/// passing the c_dfDIJoystick2 global data format variable.
	/// If an axis is in relative mode, the appropriate member contains the 
	/// change in position. If it is in absolute mode, the member contains
	/// the absolute axis position.
	/// Some drivers report the centered position of the POV indicator as 65,535. 
	/// Determine whether the indicator is centered as follows:
	///
	/// BOOL POVCentered = (LOWORD(dwPOV) == 0xFFFF);
	/// 



private:


	/// \brief	ctor
	CJoypad(void);

	/// \brief	dtor
	~CJoypad(void);

	// the two enumeration callbacks are declared as friends so thay can
	// initialise the devices.
	//
	friend BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE*,VOID*);
	friend BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE*,VOID*);
	friend bool InitInput();
	friend bool UpdateInputState();
	friend void FreeInput();


	/// this variable holds packed bit flags for the axes that this device
	/// supports
	///
	unsigned int   m_iAxes;

	/// the number of buttons on the device
	///
	unsigned short m_nButtons;

	/// the number of point of view controls on the device
	/// 
	unsigned short m_nPovs;

	/// This is a pointer to the device
	/// 
	LPDIRECTINPUTDEVICE8 m_pJoystick;    

	/// The directInput joystick state. This holds info about the 
	/// buttons pressed and d-pad / stick directions
	/// 
	DIJOYSTATE2 m_js; 


	std::string m_Product;
	std::string m_Instance;
};


#else


//----------------------------------------------------------------------------------------
/// The Linux implimentation of the joypad interface class.
///
class CJoypad : public XInputJoypad
{
public:


	////////////////////////////////////////////////////////////////////////// DEVICE INFO

	const char* const InstanceName() const {
		return name;
	}
	const char* const ProductName() const {
		return name;
	}

	////////////////////////////////////////////////////////////////////////// DEVICE CAPABILITIES

	///
	/// this function returns the number of buttons available on the joypad
	/// 
	const unsigned short NumButtons() const {
		return m_nButtons;
	}

	///
	/// this function returns the number of buttons available on the joypad
	///
	const unsigned short NumPovs() const {
		return 0;
	}

	///
	/// the following functions allow you to check to see if the specified
	/// axis is available on the joypad.
	///

	bool HasX() const {
		return m_iAxes>=1;
	}
	bool HasY() const {
		return m_iAxes>=2;
	}
	bool HasZ() const {
		return m_iAxes>=3;
	}
	bool HasXrot() const {
		return m_iAxes>=4;
	}
	bool HasYrot() const {
		return m_iAxes>=5;
	}
	bool HasZrot() const {
		return m_iAxes>=6;
	}
	bool HasExtra1() const {
		return m_iAxes>=7;
	}
	bool HasExtra2() const {
		return m_iAxes>=8;
	}

	//////////////////////////////////////////////////////////////////////////	BUTTONS


	/// Array of buttons. The high-order bit of the byte is set if the
	/// corresponding button is down, and clear if the button is up or does
	/// not exist.
	///
	const bool Button(const unsigned int& i) const {
		return Buttons[i] != 0;
	}


	//////////////////////////////////////////////////////////////////////////	X AXIS

	///	X-axis, usually the left-right movement of a stick.
	const LONG Xaxis() const {
		return Axes[0];
	}
	/// X-axis velocity.
	const LONG Xvelocity() const {
		return 0;
	}
	/// X-axis acceleration.
	const LONG Xacceleration() const {
		return 0;
	}
	/// X-axis force.
	const LONG Xforce() const {
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////	Y AXIS



	///	Y-axis, usually the forward-backward movement of a stick.
	const LONG Yaxis() const {
		return Axes[1];
	}
	/// Y-axis velocity.
	const LONG Yvelocity() const {
		return 0;
	}
	/// Y-axis acceleration.
	const LONG Yacceleration() const {
		return 0;
	}
	/// Y-axis force.
	const LONG Yforce() const {
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////	Z AXIS

	///	Z-axis, often the throttle control. If the joystick does not have this
	/// axis, the value is 0.
	const LONG Zaxis() const {
		return Axes[2];
	}
	/// Z-axis velocity.
	const LONG Zvelocity() const {
		return 0;
	}
	/// Z-axis acceleration.
	const LONG Zacceleration() const {
		return 0;
	}
	/// Z-axis force.
	const LONG Zforce() const {
		return 0;
	}



	//////////////////////////////////////////////////////////////////////////	X AXIS ROTATION

	///	X-axis rotation. If the joystick does not have this axis, the value is 0.
	const LONG Xrot() const {
		return Axes[3];
	}
	/// X-axis angular velocity.
	const LONG XrotVelocity() const {
		return 0;
	}
	/// X-axis angular acceleration.
	const LONG XrotAcceleration() const {
		return 0;
	}
	/// X-axis torque.
	const LONG XrotForce() const {
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////	Y AXIS ROTATION

	///	Y-axis rotation. If the joystick does not have this axis, the value is 0.
	const LONG Yrot() const {
		return Axes[4];
	}
	/// Y-axis angular velocity.
	const LONG YrotVelocity() const {
		return 0;
	}
	/// Y-axis angular acceleration.
	const LONG YrotAcceleration() const {
		return 0;
	}
	/// Y-axis torque.
	const LONG YrotForce() const {
		return 0;
	}



	//////////////////////////////////////////////////////////////////////////	Z AXIS ROTATION

	///	Z-axis rotation (often called the rudder). If the joystick does not have
	/// this axis, the value is 0.
	const LONG Zrot() const {
		return Axes[5];
	}
	/// Z-axis angular velocity. .
	const LONG ZrotVelocity() const {
		return 0;
	}
	/// Z-axis angular acceleration.
	const LONG ZrotAcceleration() const {
		return 0;
	}
	/// Z-axis torque.
	const LONG ZrotForce() const {
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////	2 ADDITIONAL AXES

	///	Two additional axis values (formerly called the u-axis and v-axis) whose
	/// semantics depend on the joystick. Use the IDirectInputDevice8::GetObjectInfo
	/// method to obtain semantic information about these values.
	const LONG ExtraAxes(const unsigned int& i) const {
		return Axes[6+i];
	}
	//Extra axis velocities.
	const LONG ExtraVelocities(const unsigned int& i) const {
		return 0;
	}
	//Extra axis accelerations.
	const LONG ExtraAccelerations(const unsigned int& i) const {
		return 0;
	}
	//Extra axis forces.
	const LONG ExtraForces(const unsigned int& i) const {
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////	POV

	/// Direction controllers, such as point-of-view hats. The position is
	/// indicated in hundredths of a degree clockwise from north (away from the
	/// user). The center position is normally reported as -1; but see Remarks.
	/// For indicators that have only five positions, the value for a controller
	/// is -1, 0, 9,000, 18,000, or 27,000.
	const DWORD POV(const unsigned int& i) const {
		return 0;
	}


private:

	int fd;

	/// this variable holds packed bit flags for the axes that this device
	/// supports
	///
	unsigned char m_iAxes;

	/// the number of buttons on the device
	///
	unsigned char m_nButtons;

	int version;
	char name[128];

	/// \brief	ctor
	CJoypad(void);

	/// \brief	dtor
	~CJoypad(void);

	short Axes[10];
	bool Buttons[20];

	bool Update();

	//
	friend bool InitInput();
	friend bool UpdateInputState();
	friend void FreeInput();

};


#endif

#endif
