// 01SimpleExample 
//
// This tutorial shows a simple example of 
// how to use OpenSG together with GLUT to create a little
// interactive scene viewer.
// taken from OpenSG Tutorials

#include <OpenSG/OSGGLUT.h>
// General OpenSG configuration, needed everywhere
#include <OpenSG/OSGConfig.h>
// Methods to create simple geometry: boxes, spheres, tori etc.
#include <OpenSG/OSGSimpleGeometry.h>
// The GLUT-OpenSG connection class
#include <OpenSG/OSGGLUTWindow.h>
// A little helper to simplify scene management and interaction
#include <OpenSG/OSGSimpleSceneManager.h>

// The SimpleSceneManager to manage simple applications
OSG::SimpleSceneManagerRefPtr mgr;
// forward declaration 
int setupGLUT( int *argc, char *argv[] );
void sortNumbers(int numbers[],int length);

// Initialize GLUT & OpenSG and set up the scene
int main(int argc, char **argv)
{
	//sortNumbers function
	int numberArray[] = {2, 13, 56, 8, 22, 16, 24, 89, 11, 90}; 
	sortNumbers(numberArray,10);

    // OSG init
    OSG::osgInit(argc,argv);

    // GLUT init
    int winid = setupGLUT(&argc, argv);

    // open a new scope, because the pointers gwin and scene below should
    // go out of scope before entering glutMainLoop.
    // Otherwise OpenSG will complain about objects being alive after shutdown.
    {
        // the connection between GLUT and OpenSG
        OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();
    
        // create the scene
        OSG::NodeRefPtr scene = OSG::makeBox(1,1,1,2,2,2);
    
        OSG::commitChanges();
    
        // create the SimpleSceneManager helper
        mgr = OSG::SimpleSceneManager::create();
    
        // tell the manager what to manage
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
    
        // show the whole scene
        mgr->showAll();
    }
    
    // GLUT main loop
    glutMainLoop();

    return 0;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
	mgr->redraw();
}

// react to size changes
void reshape(int w, int h)
{
    mgr->resize(w, h);
    glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
    if (!state)
      mgr->mouseButtonPress(button, x, y);
	        
    glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 'e':
        {
            // clean up global variables
            mgr = NULL;
        
            OSG::osgExit();
            exit(0);
        }
        break;

        case 's':
        {
            mgr->setStatistics(!mgr->getStatistics());
        }
        break;
    }
}

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("01SimpleExample");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    return winid;
}
void sortNumbers(int numbers[],int length){

}
