#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

#if _WIN32
#include <windows.h>
#endif

#include  "Shared_Memory/Shared_Memory.h"
#include  "Plugin_API/pluginmanager.h"
#include "../Logger_API/easylogging++.h"


#ifdef OPENGL_GUI
#include  "Gui/gui.h"
#include "Gui/input_singleton.h"

pthread_mutex_t lock;

void InitGL ( int Width, int Height )
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        LOG(WARNING) << "Gui mutex lock init failed";
        return;
    }
    else
    {
        LOG(TRACE) << "Gui mutex lock initialized";
    }
    
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glClearColor ( 0.7f, 0.7f, 0.7f, 1.0f );
    glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
    glColor3f ( 1.0f, 1.0f, 1.0f );
    glPointSize ( 50 );
    glLineWidth ( 2 );
    glEnable ( GL_CULL_FACE ); //disable this befor drawing gui
    glEnable ( GL_BLEND );
    glEnable ( GL_TEXTURE_2D );
    glEnable ( GL_DEPTH_TEST );
    glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
    glBlendEquation ( GL_FUNC_ADD );
    glDepthFunc ( GL_LEQUAL );
    glEnable ( GL_ALPHA_TEST );
    glAlphaFunc ( GL_GREATER, 0.001f );
    glPolygonMode ( GL_FRONT, GL_FILL );
}

void DrawGLScene ( void )
{
    pthread_mutex_lock(&lock);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    Gui::getInstance().Draw();
    glutSwapBuffers();
    pthread_mutex_unlock(&lock);
}

void shutdownApp (void)
{
    PluginManager::getInstance().UnloadAll();
    glutDestroyWindow (1);
    _exit( 0 );
}

void my_function(int sig){ 
	LOG(TRACE) << "SIGINT Signal recived... \nStarting shutdown sequence.\n\n";
	shutdownApp();	
}

void idle_func ( void )
{
    Gui::getInstance().proccesInput();
    glFlush();
    glutPostRedisplay ( );

}

void processMouse ( int button, int state, int x, int y )
{
    InputSingleton::getInstance().ProcessMouse(button, state, x, y, glutGetModifiers());
}


void processMouseActiveMotion ( int x, int y )
{
    InputSingleton::getInstance().ActiveMotion(x,y);
}


void processMousePassiveMotion ( int x, int y )
{
    InputSingleton::getInstance().PassiveMotion(x,y);
}


void processMouseEntry ( int state )
{

}

void keyPressed ( unsigned char key, int x, int y )
{
    InputSingleton::getInstance().KeyPressed(key, x, y, glutGetModifiers());
    switch ( key )
    {
    case 27:
		LOG(TRACE) << "\n \nEscape button pressed... \nStarting shutdown sequence.\n\n";
        shutdownApp();
        break;
    }
}

void keyReleased(unsigned char key, int x, int y)
{
    InputSingleton::getInstance().KeyReleased(key, x, y);
}

void specialKeyPressed ( int key, int x, int y )
{
    InputSingleton::getInstance().SpecialKeyPressed(key, x, y, glutGetModifiers());
}

void specialKeyReleased ( int key, int x, int y )
{
    InputSingleton::getInstance().SpecialKeyReleased (key, x, y);
}

void reshape ( int w, int h )
{
    if ( w <= 0 ) w = 1;
    if ( h <= 0 ) h = 1;
    
    glViewport ( 0, 0, ( GLsizei ) w, ( GLsizei ) h );

    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    gluPerspective ( 43.0, ( GLfloat ) w/ ( GLfloat ) h, 0.1, 1000.0 );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();
    Gui::getInstance().resize(w,h);
}
#else
void shutdownApp (void)
{
    PluginManager::getInstance().UnloadAll();
    _exit( 0 );
}

void my_function(int sig){ 
	LOG(TRACE) << "SIGINT Signal recived... \nStarting shutdown sequence.\n\n";
	shutdownApp();	
}
#endif

int main(int argc, char* argv[])
{
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    el::Configurations conf("../conf/logger.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    LOG(TRACE) << "Logger Started";
    
    signal (SIGINT,my_function);

#ifdef OPENGL_GUI
    LOG(TRACE) << "Using OpenGL GUI";
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );
    glutInitWindowSize ( WIDTH , HEIGHT);
    glutInitWindowPosition ( 0, 0 );
    glutCreateWindow ( "Markovito - INAOE" );
    glutDisplayFunc ( &DrawGLScene );
    //glutFullScreen();
    glutIdleFunc ( &DrawGLScene );
    glutIdleFunc ( idle_func );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( &keyPressed );
    glutKeyboardUpFunc ( &keyReleased);
    glutSpecialFunc ( &specialKeyPressed );
    glutSpecialUpFunc( &specialKeyReleased );
    InitGL ( WIDTH , HEIGHT);
    Gui::getInstance().initialize();
    LOG(TRACE) << "OpenGL GUI initialized";
#else
	LOG(TRACE) << "NOT Using OpenGL GUI";
#endif
	
    Shared_Memory::getInstance();
    PluginManager::getInstance().LoadFromFile("../../conf/plugins");
    PluginManager::getInstance().LoadFromFile("../conf/plugins");
    PluginManager::getInstance().InitializeAll();
    PluginManager::getInstance().ExecuteAll();

#ifdef OPENGL_GUI
    glutMouseFunc ( processMouse );
    glutMotionFunc ( processMouseActiveMotion );
    glutPassiveMotionFunc ( processMousePassiveMotion );
    glutEntryFunc ( processMouseEntry );
    glutMainLoop();
#else
    for(;;);
#endif
    return 0;
}
