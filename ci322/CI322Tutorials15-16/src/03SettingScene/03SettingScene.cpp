// 03SettingScene 
//
// This tutorial will 
// set up a simple interactive scene 
// 

// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;

// GLUT Menu items
enum MENU_ITEMS
{
	WIREFRAME,
	SOLID,
	VERTEX,
};



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


// GLUT menu handling function definition
void menu(int item)
{
	switch (item)
	{
	case VERTEX:{
		glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
				}
				break;
	case WIREFRAME: {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				}
				break;
	case SOLID: {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				}
				break;
	default:
		{ }
		break;
	}
	glutPostRedisplay();
	return;
}
void createMenu(){
	glutCreateMenu(menu);
	// Add menu items
	glutAddMenuEntry("Show vertices", VERTEX);
	glutAddMenuEntry("Show wireframe", WIREFRAME);
	glutAddMenuEntry("Show solid", SOLID);
	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	return;
}



// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	int winid = glutCreateWindow("03SettingScene Tutorial");


	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	createMenu();
	return winid;
}



int main(int argc, char **argv)
{
	// initialise GLUT
	int winid = setupGLUT(&argc, argv);
	// initialise OpenSG
	OSG::osgInit(argc,argv);
	// create a OSGGLUT window
	OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
	gwin->setGlutId(winid);
	gwin->init();
	// create the SimpleSceneManager helper




	// tell the manager what to manage

	mgr = OSG::SimpleSceneManager::create();
	mgr->setWindow(gwin);

	//read the file which will be passed as an argument
	OSG::NodeRefPtr file;
	if(argc > 1)
	{
		for (int i=1; i<argc ; i++){
			std::string option = argv[i];
			if (option == "-f")
				file = OSG::SceneFileHandler::the()->read(argv[i+1]);
		}
	}

	mgr->setRoot(file);

	OSG::commitChanges();
	// show the whole 
	mgr->showAll();

	// GLUT main loop
	glutMainLoop();
	return 0;


}

