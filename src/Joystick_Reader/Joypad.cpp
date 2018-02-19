#include "Joypad.h"

#include <vector>

#ifdef WIN32

//////////////////////////////////////////////////////////////////////////
//	Defines, constants, and global variables
//////////////////////////////////////////////////////////////////////////

#define SAFE_DELETE(p)  { delete (p);     (p)=NULL; }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

CJoypad::CJoypad(void) : m_iAxes(0),m_nButtons(0),m_nPovs(0),m_pJoystick(0)
{
	memset(&m_js,0,sizeof(DIJOYSTATE2));
}

CJoypad::~CJoypad(void)
{
	//	Unacquire the device one last time just in case 
	//	the app tried to exit while the device is still acquired.
	//
	if( m_pJoystick ) 
		m_pJoystick->Unacquire();

	//	Release any DirectInput objects.
	//
	SAFE_RELEASE( m_pJoystick );
}

bool CJoypad::Update()
{

	//	Poll the device to read the current state
	//
	HRESULT hr = m_pJoystick->Poll(); 

	if( FAILED(hr) )  
	{
		//	DInput is telling us that the input stream has been
		//	interrupted. We aren't tracking any state between polls, so
		//	we don't have any special reset that needs to be done. We
		//	just re-acquire and try again.
		//
		do 
		{
			hr = m_pJoystick->Acquire();
		} 
		while( hr == DIERR_INPUTLOST );

		//	hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		//	may occur when the app is minimized or in the process of 
		//	switching, so just try again later 
		//
		return true; 
	}


	// Get the input's device state
	//
	if( FAILED( hr = m_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_js ) ) )
	{
		std::cerr << "ERROR: [CJoypad]  The device should have been acquired during the Poll()" << std::endl;
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//	Function Prototypes
//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );




LPDIRECTINPUT8       g_pDI              = NULL; 
LPDIRECTINPUTDEVICE8 g_pJoystick		= NULL;
std::vector< XInputJoypad* > g_aJoypads;

CJoypad* g_jp=NULL;



//////////////////////////////////////////////////////////////////////////
/// \brief	Initialize the DirectInput variables.
/// \return	true if OK
///
bool InitInput()
{
	HRESULT hr;

	//	Register with the DirectInput subsystem and get a pointer
	//	to a IDirectInput interface we can use.
	//	Create a DInput object
	//
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
		return false;

	//	Look for a simple joystick we can use for this sample program.
	//
	if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL, DIEDFL_ATTACHEDONLY ) ) )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
/// \brief	Called once for each enumerated joystick. If we find one, create a
///			device interface on it so we can play with it.
/// \param	pdidInstance	-
/// \param	pContext		-
/// \return	DIENUM_CONTINUE or DIENUM_STOP
/// 

BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
									VOID* pContext )
{
	HRESULT hr;

	// Obtain an interface to the enumerated joystick.
	hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if( FAILED(hr) ) 
		return DIENUM_CONTINUE;



	//	Set the data format to "simple joystick" - a predefined data format 
	//
	//	A data format specifies which controls on a device we are interested in,
	//	and how they should be reported. This tells DInput that we will be
	//	passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	//
	if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
	{
		g_pJoystick		= NULL;
		return false;
	}

	//	--- > This will fail with glut under Win32
	//
	//	Set the cooperative level to let DInput know how this device should
	//	interact with the system and with other DInput applications.
	//
/*	if( FAILED( hr = g_pJoystick->SetCooperativeLevel( NULL, DISCL_EXCLUSIVE |
		DISCL_FOREGROUND ) ) )
	{
		g_pJoystick		= NULL;
		return false;
	}*/

	g_jp = new CJoypad;

	g_jp->m_pJoystick = g_pJoystick;

	//	Enumerate the joystick objects. The callback function enabled user
	//	interface elements for objects that are found, and sets the min/max
	//	values property for discovered axes.
	//
	if( FAILED( hr = g_jp->m_pJoystick->EnumObjects( EnumObjectsCallback, NULL, DIDFT_ALL ) ) )
	{
		g_pJoystick		= NULL;
		delete g_jp;
		return false;
	}

	DIDEVCAPS diDevCaps;
	//	query the device capabilities to find out how many buttons we have
	// 
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED( g_jp->m_pJoystick->GetCapabilities(&diDevCaps))) 
	{
		g_pJoystick		= NULL;
		delete g_jp;
		return false;
	}

	g_jp->m_nButtons = static_cast<unsigned short>(diDevCaps.dwButtons);

	g_aJoypads.push_back(g_jp);

	g_pJoystick		= NULL;

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_CONTINUE;
}



//////////////////////////////////////////////////////////////////////////
/// \brief	Callback function for enumerating objects (axes, buttons, POVs) on a 
///			joystick. This function enables user interface elements for objects
///			that are found to exist, and scales axes min/max values.
/// \param	pdidInstance	-
/// \param	pContext		-
/// \return	DIENUM_CONTINUE or DIENUM_STOP
/// 
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
								  VOID* pContext )
{
	static int nSliderCount = 0;  // Number of returned slider controls
	static int nPOVCount = 0;     // Number of returned POV controls

	// For axes that are returned, set the DIPROP_RANGE property for the
	// enumerated axis in order to scale min/max values.
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow        = DIPH_BYID; 
		diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin              = -32766; 
		diprg.lMax              = +32766; 

		// Set the range for the axis
		if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
			return DIENUM_STOP;

	}

    // Set the UI to reflect what objects the joystick supports
    if (pdidoi->guidType == GUID_XAxis)
    {
		g_jp->m_iAxes |= XInputJoypad::X_AXIS;
    }
    if (pdidoi->guidType == GUID_YAxis)
    {
		g_jp->m_iAxes |= XInputJoypad::Y_AXIS;
    }
    if (pdidoi->guidType == GUID_ZAxis)
    {
		g_jp->m_iAxes |= XInputJoypad::Z_AXIS;
    }
    if (pdidoi->guidType == GUID_RxAxis)
    {
		g_jp->m_iAxes |= XInputJoypad::X_ROT_AXIS;
    }
    if (pdidoi->guidType == GUID_RyAxis)
    {
		g_jp->m_iAxes |= XInputJoypad::Y_ROT_AXIS;
    }
    if (pdidoi->guidType == GUID_RzAxis)
    {
		g_jp->m_iAxes |= XInputJoypad::Z_ROT_AXIS;
    }
    if (pdidoi->guidType == GUID_Slider)
    {
        switch( nSliderCount++ )
        {
            case 0 :
				g_jp->m_iAxes |= XInputJoypad::SLIDER_1;
                break;

            case 1 :
				g_jp->m_iAxes |= XInputJoypad::SLIDER_1;
				g_jp->m_iAxes |= XInputJoypad::SLIDER_2;
                break;
        }
    }
    if (pdidoi->guidType == GUID_POV)
    {
		g_jp->m_nPovs = nPOVCount+1;
    }

	return DIENUM_CONTINUE;
}




//////////////////////////////////////////////////////////////////////////
/// \brief	Get the input device's state
/// \return	true if OK
///
bool UpdateInputState()
{
	std::vector< XInputJoypad* >::iterator it = g_aJoypads.begin();

	for( ; it != g_aJoypads.end(); ++it )
	{
		(*it)->Update();
	}
	return true;
}




//////////////////////////////////////////////////////////////////////////
/// \brief	Cleans up Direct Input
///
void FreeInput()
{
	//	Unacquire the device one last time just in case
	//	the app tried to exit while the device is still acquired.
	//
	if( g_pJoystick )
		g_pJoystick->Unacquire();

	//	Release any DirectInput objects.
	//

	std::vector< XInputJoypad* >::iterator it = g_aJoypads.begin();

	for( ; it != g_aJoypads.end(); ++it )
	{
		delete *it;
	}

	SAFE_RELEASE( g_pDI );
}

#else

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>


std::vector< XInputJoypad* > g_aJoypads;


/// \brief	dtor
CJoypad::~CJoypad(void) {
	close(fd);
}

#define NAME_LENGTH 128

CJoypad::CJoypad() : m_iAxes(2), m_nButtons(2) {
	version = 0x000800;
	std::cout << name << "Unknown" << std::endl;
}
bool CJoypad::Update() {
	struct js_event js;
	while ( read(fd,&js,sizeof(struct js_event)) >= 0 )  {
		switch(js.type) {

		case JS_EVENT_BUTTON:
			Buttons[ js.number ] = js.value;
			break;

		case JS_EVENT_AXIS:
			Axes[ js.number ] = js.value;
			break;

		default:
			return false;
		};
	}
	return true;
}

bool UpdateInputState()
{
	std::vector< XInputJoypad* >::iterator it = g_aJoypads.begin();
	for( ; it != g_aJoypads.end(); ++it )
	{
		(*it)->Update();
	}
}

//////////////////////////////////////////////////////////////////////////
/// \brief	Initialize the DirectInput variables.
/// \return	true if OK
///
bool InitInput()
{
	bool ret=false;
	int fd;
	if ( (fd = open("/dev/input/js0", O_RDONLY)) > 0 ) 
	{
		CJoypad* ptr = new CJoypad;
		ioctl(fd, JSIOCGVERSION, &ptr->version);
		ioctl(fd, JSIOCGAXES, &ptr->m_iAxes);
		ioctl(fd, JSIOCGBUTTONS, &ptr->m_nButtons);
		ioctl(fd, JSIOCGNAME(NAME_LENGTH), ptr->name);
		fcntl(fd, F_SETFL, O_NONBLOCK);
		ptr->fd = fd;
		g_aJoypads.push_back(ptr);
		ret=true;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/// \brief	Cleans up Direct Input
///
void FreeInput()
{
	std::vector< XInputJoypad* >::iterator it = g_aJoypads.begin();
	for( ; it != g_aJoypads.end(); ++it )
	{
		delete (*it);
	}
	g_aJoypads.resize(0);
}

#endif


unsigned int NumJoypads() {
	return static_cast<unsigned int>( g_aJoypads.size() );
}

XInputJoypad* GetJoypad(const unsigned int ref) {
	if( ref < NumJoypads() )
		return g_aJoypads[ref];
	return NULL;
}
