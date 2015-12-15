// 04 Setting Scene without a SimpleSceneManager
//
// This tutorial will 
// set up a simple interactive scene 
// 
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSolidBackground.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGPointLight.h>
#include <OpenSG/OSGSpotLight.h>
#include <OpenSG/OSGPerspectiveCamera.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGRenderAction.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGGradientBackground.h>
#include <OpenSG/OSGTextureBackground.h>
#include <OpenSG/OSGTextureObjChunk.h>
#include <OpenSG/OSGImage.h>
#include <OpenSG/OSGImageForeground.h>
#include <OpenSG/OSGFileGrabForeground.h>
#include <OpenSG/OSGMatrixUtility.h>
#include <OpenSG/OSGMatrixUtility.h>
#include <OpenSG/OSGNavigator.h>
#include <OpenSG/OSGSceneFileHandler.h>


OSG::NodeRecPtr scene;
OSG::PerspectiveCameraRecPtr camera;
OSG::PerspectiveCameraRecPtr camera1;
OSG::PerspectiveCameraRecPtr camera2;
OSG::ViewportRecPtr viewport;
OSG::ViewportRecPtr viewport1;
OSG::ViewportRecPtr viewport2;
OSG::WindowRecPtr window;
OSG::NodeRecPtr leftCamBeacon, lightBeacon, lightNode, rightCamBeacon, centreCamBeacon;
OSG::RenderActionRefPtr renderAction;
OSG::Navigator navigator, navigator2, navigator1;
OSG::Int16 lastx;
OSG::Int16 lasty;
OSG::UInt16 mousebuttons;
OSG::NodeRefPtr file;


int setupGLUT(int *argc, char *argv[]);
void cameraAlongZ();
void cameraAlongX();
void cameraAlongY();

OSG::NodeTransitPtr createScenegraph(int argc, char **argv)
{
	//create geometry 
	//read the file which will be passed as an argument
	if(argc > 1)
	{
		for (int i=1; i<argc ; i++){
			std::string option = argv[i];
			if (option == "-f")
				file = OSG::SceneFileHandler::the()->read(argv[i+1]);
		}
	}
	if(file==NULL) file = OSG::makeSphere(3,3);

	//create the light Beacon
	lightBeacon = OSG::Node::create();

	//create Transformations
	OSG::TransformRecPtr lightTrans = OSG::Transform::create();
	OSG::Matrix lightM;

	OSG::Pnt3f fromLight(0,15,0);
	OSG::Pnt3f atLight(0,0,0);
	OSG::Vec3f upLight(0,0,1);
	OSG::MatrixLookAt(lightM, fromLight, atLight, upLight);
	lightTrans->setMatrix(lightM);
	lightBeacon->setCore(lightTrans);

	//create the light source
	OSG::DirectionalLightRecPtr dLight = OSG::DirectionalLight::create();
	dLight->setDirection(OSG::Vec3f(0,1,2));
	//color information
	dLight->setDiffuse(OSG::Color4f(1,1,1,1));
	dLight->setAmbient(OSG::Color4f(0.2,0.2,0.2,1));
	dLight->setSpecular(OSG::Color4f(1,1,1,1));
	//set the beacon
	dLight->setBeacon(lightBeacon);

	// create the node that will contain the light source
	lightNode = OSG::Node::create();
	lightNode->setCore(dLight);
	lightNode->addChild(file);

	// now create the root and add all children
	OSG::NodeRecPtr root = OSG::Node::create();
	root->setCore(OSG::Group::create());
	root->addChild(lightNode);

	return OSG::NodeTransitPtr(root);
}

int main(int argc, char **argv)
{
	OSG::osgInit(argc,argv);

	{
		int winid = setupGLUT(&argc, argv);

		OSG::GLUTWindowRecPtr gwin = OSG::GLUTWindow::create();
		gwin->setGlutId(winid);
		gwin->setSize(300,300);
		window = gwin;
		window->init();

		scene = createScenegraph(argc,argv);

		//create camera Beacon
		leftCamBeacon = OSG::Node::create();   
		rightCamBeacon = OSG::Node::create();
		centreCamBeacon = OSG::Node::create();
		//create Transformations
		OSG::TransformRecPtr leftCamTrans =  OSG::Transform::create();
		OSG::TransformRecPtr rightCamTrans =  OSG::Transform::create();
		OSG::TransformRecPtr centreCamTrans =  OSG::Transform::create();

		OSG::Matrix leftM, rightM, lightM, centreM;
		//matrix to define viewer position (from), a point that should be in the center of the screen (at) and the direction that should be up on the screen (up). 
		OSG::Pnt3f fromLeftCamera(0,0,10);
		OSG::Pnt3f fromRightCamera(0,0,10);
		OSG::Pnt3f fromCentreCamera(0,0,10);
		
		OSG::Pnt3f atCamera(0,0,0);
		OSG::Vec3f upCamera(0,1,0);
		
		OSG::MatrixLookAt(leftM, fromLeftCamera, atCamera, upCamera);
		OSG::MatrixLookAt(rightM, fromRightCamera, atCamera, upCamera);
		OSG::MatrixLookAt(centreM, fromCentreCamera, atCamera, upCamera);

		leftCamTrans->setMatrix(leftM);
		rightCamTrans->setMatrix(rightM);
		centreCamTrans->setMatrix(centreM);
		
		leftCamBeacon->setCore(leftCamTrans);
		rightCamBeacon->setCore(rightCamTrans);
		centreCamBeacon->setCore(centreCamTrans);

	/*	//create Transformations
		OSG::TransformRecPtr camTrans2 =  OSG::Transform::create();
		OSG::Matrix camM2;
		//matrix to define viewer position (from), a point that should be in the center of the screen (at) and the direction that should be up on the screen (up). 
		OSG::Pnt3f fromCamera2(100,0,0);
		OSG::Pnt3f atCamera2(10,0,0);
		OSG::Vec3f upCamera2(0,1,0);
		OSG::MatrixLookAt(camM2, fromCamera2, atCamera2, upCamera2);
		camTrans2->setMatrix(camM2);
		camBeacon2->setCore(camTrans2);*/

		//select the type of camera we want for the scene
		camera = OSG::PerspectiveCamera::create();
		camera->setBeacon(leftCamBeacon);
		camera->setFov(OSG::osgDegree2Rad(60));
		camera->setNear(0.1f);
		camera->setFar(10000.f);

		camera1 = OSG::PerspectiveCamera::create();
		camera1->setBeacon(rightCamBeacon);
		camera1->setFov(OSG::osgDegree2Rad(60));
		camera1->setNear(0.1f);
		camera1->setFar(10000.f);
		
		camera2 = OSG::PerspectiveCamera::create();
		camera2->setBeacon(centreCamBeacon);
		camera2->setFov(OSG::osgDegree2Rad(60));
		camera2->setNear(0.1f);
		camera2->setFar(10000.f);

		//read an image to use as a background
		OSG::ImageRecPtr bkgImage = OSG::Image::create();
		bkgImage->read("c:/build/sample/front.jpg");

		// make a texture from the image
		OSG::TextureObjChunkRecPtr bkgTex = OSG::TextureObjChunk::create();
		bkgTex->setImage(bkgImage);

		OSG::TextureBackgroundRecPtr texBkg = OSG::TextureBackground::create();

		texBkg->setTexture(bkgTex);

		//now the viewports
		viewport = OSG::Viewport::create();
		viewport->setCamera(camera);
		viewport->setBackground(texBkg);
		viewport->setRoot(scene);
		viewport->setSize(0,0,.33,1);

		viewport1 = OSG::Viewport::create();
		viewport1->setCamera(camera1);
		viewport1->setBackground(texBkg);
		viewport1->setRoot(scene);
		viewport1->setSize(.33,0,.66,1);

		viewport2 = OSG::Viewport::create();
		viewport2->setCamera(camera2);
		viewport2->setBackground(texBkg);
		viewport2->setRoot(scene);
		viewport2->setSize(.66,0,1,1);

		// add an logo to the viewport
		//load the logo image file
		OSG::ImageRecPtr frgImage = OSG::Image::create();
		frgImage->read("c:/build/sample/logo.png");

		OSG::ImageForegroundRecPtr imgFrg = OSG::ImageForeground::create();
		//NOTE: the position values are between 0 and 1
		//and are relative to the viewport!
		imgFrg->addImage(frgImage, OSG::Pnt2f(0.1,0));

		//add the created foreground by appending it to
		//the vieports foreground multifield
		viewport->editMFForegrounds()->push_back(imgFrg);


		//navigator
		navigator.setMode(OSG::Navigator::TRACKBALL);
		navigator.setViewport(viewport);
		navigator.setCameraTransformation(leftCamBeacon);
		navigator.setMotionFactor(0.2f);
		OSG::Vec3f up(0,1,0);
		OSG::Pnt3f at(0,0,0);
		OSG::Pnt3f from(0,  0, 10);
		navigator.set(from, at, up);

		//navigator2
		navigator2.setMode(OSG::Navigator::TRACKBALL);
		navigator2.setViewport(viewport1);
		navigator2.setCameraTransformation(rightCamBeacon);
		navigator2.setMotionFactor(0.2f);
		OSG::Vec3f up1(1,0,1);
		OSG::Pnt3f at1(0,0,0);
		OSG::Pnt3f from1(10,  0, 10);
		navigator2.set(from1, at1, up1);

			//navigator1
		navigator1.setMode(OSG::Navigator::TRACKBALL);
		navigator1.setViewport(viewport2);
		navigator1.setCameraTransformation(centreCamBeacon);
		navigator1.setMotionFactor(0.2f);
		OSG::Vec3f up2(1,0,1);
		OSG::Pnt3f at2(0,0,0);
		OSG::Pnt3f from2(0,  0, -10);
		navigator1.set(from2, at2, up2);

		//the render action 
		renderAction = OSG::RenderAction::create();

		//create the window now
		window->addPort(viewport);
		window->addPort(viewport1);
		window->addPort(viewport2);
		OSG::commitChanges();
	}

	glutMainLoop();


	return 0;
}

void reshape(int w, int h)
{
	window->resize(w, h);
	glutPostRedisplay();
}

void display(void)
{
	navigator.updateCameraTransformation();
	navigator1.updateCameraTransformation();
	navigator2.updateCameraTransformation();
	window->render(renderAction);
	OSG::commitChanges();
}

void mouse(int button, int state, int x, int y)
{
	if(state)
	{
		switch(button)
		{
		case 0:
			navigator.buttonRelease(OSG::Navigator::LEFT_MOUSE, x, y);
			break;
		case 1:
			navigator.buttonRelease(OSG::Navigator::MIDDLE_MOUSE, x, y);
			break;
		case 2:
			navigator.buttonRelease(OSG::Navigator::RIGHT_MOUSE, x, y);
			break;
		case 3:
			navigator.buttonRelease(OSG::Navigator::UP_MOUSE, x, y);
			break;
		case 4:
			navigator.buttonRelease(OSG::Navigator::DOWN_MOUSE, x, y);
			break;
		}
		mousebuttons &= ~(1 << button);
		lastx = x;
		lasty = y;
	}

	else
	{
		switch(button)
		{
		case 0:
			navigator.buttonPress(OSG::Navigator::LEFT_MOUSE, x, y);
			break;
		case 1:
			navigator.buttonPress(OSG::Navigator::MIDDLE_MOUSE, x, y);
			break;
		case 2:
			navigator.buttonPress(OSG::Navigator::RIGHT_MOUSE, x, y);
			break;
		case 3:
			navigator.buttonPress(OSG::Navigator::UP_MOUSE, x, y);
			break;
		case 4:
			navigator.buttonPress(OSG::Navigator::DOWN_MOUSE, x, y);
			break;
		}
		mousebuttons |= 1 << button;
		lastx = x;
		lasty = y;
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{
	//mgr->mouseMove(x, y);
	if(mousebuttons)
		navigator.moveTo(x, y);
	lastx = x;
	lasty = y;
	glutPostRedisplay();
}

void keyboard(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
		{
			// clean up global variables
			scene          = NULL;
			window         = NULL;
			camera		   = NULL;
			viewport       = NULL;
			leftCamBeacon      = NULL;
			rightCamBeacon      = NULL;
			centreCamBeacon      = NULL;
			lightBeacon    = NULL;
			lightNode      = NULL;
			renderAction   = NULL;

			OSG::osgExit();
			exit(1);
		}
		break;

	}
}

int setupGLUT(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	int winid = glutCreateWindow("04Setting Complex Scene ");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	return winid;
}
