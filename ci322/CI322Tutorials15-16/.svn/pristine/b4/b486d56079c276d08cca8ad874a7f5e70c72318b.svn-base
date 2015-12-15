// 02Vectors 
//
// This tutorial will 
// show how to use vectors in OpenSG
// and visualise them in an interactive scene viewer.


#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGGeoProperties.h>

// The SimpleSceneManager to manage simple applications
OSG::SimpleSceneManagerRefPtr mgr;
// forward declarations 
int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr drawPointInScene(OSG::Pnt3f point,OSG::Color3f color);
OSG::NodeRefPtr drawVectorInScene(OSG::Vec3f vector, OSG::Color3f color);
void displayVectorValues(OSG::Vec3f vector);
int main(int argc, char **argv)
{
	
    OSG::osgInit(argc,argv);

    int winid = setupGLUT(&argc, argv);

    {
        OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();
    
        OSG::NodeRefPtr scene = OSG::Node::create();
		scene->setCore(OSG::Group::create());

		//EXAMPLE CREATING A POINT
		OSG::Pnt3f p1(0,0,0);
		scene->addChild(drawPointInScene(p1,OSG::Color3f(0,0,1)));

		//EXAMPLE CREATING A VECTOR
		OSG::Pnt3f p1v1(0,0,0);
		OSG::Pnt3f p2v1(1,0,0);
		OSG::Vec3f v1(p2v1-p1v1);
		scene->addChild(drawVectorInScene(v1,OSG::Color3f(1,1,1)));

		OSG::commitChanges();
    
        mgr = OSG::SimpleSceneManager::create();
    
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
        //mgr->showAll();
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
    
    int winid = glutCreateWindow("02Vectors Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    return winid;
}
OSG::NodeRefPtr drawPointInScene(OSG::Pnt3f point,OSG::Color3f color){

	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_POINTS);
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(1);
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	pnts->addValue(point);
	OSG::GeoVec3fPropertyRefPtr colors = OSG::GeoVec3fProperty::create();
	colors->addValue(color);
	
	geo->setTypes    (type);
	geo->setLengths  (lens);
	geo->setPositions(pnts);
	geo->setColors	 (colors);
	
    
    // assign a material to the geometry to make it visible. 
    geo->setMaterial(OSG::getDefaultUnlitMaterial());   
        
	geonode->setCore(geo);
	return geonode;
}
OSG::Pnt3f getPointAlongLine(OSG::Pnt3f pA, OSG::Pnt3f pB, OSG::Real32 distance){
			
			OSG::Vec3f vA(pA);
			OSG::Vec3f vAB(pB-pA);
			OSG::Vec3f vC = vA + distance * vAB;
			OSG::Pnt3f newPoint(vC.x(), vC.y(), vC.z());

			return newPoint;

}
OSG::NodeRefPtr drawVectorInScene(OSG::Vec3f vector,OSG::Color3f color){

	OSG::NodeRefPtr geogroup = OSG::Node::create();
	geogroup->setCore(OSG::Group::create());

	//the line
	OSG::NodeRefPtr geonode = OSG::Node::create();
	OSG::GeometryRefPtr geo = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_LINES);
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(2);
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	
	
	OSG::Pnt3f fpoint = OSG::Pnt3f(vector.x(),vector.y(),vector.z());
	OSG::Pnt3f ipoint = fpoint-fpoint;
	pnts->addValue(ipoint);
	pnts->addValue(fpoint);

	OSG::GeoVec3fPropertyRefPtr colors = OSG::GeoVec3fProperty::create();
	colors->addValue(color);
	
	geo->setTypes    (type);
	geo->setLengths  (lens);
	geo->setPositions(pnts);
	geo->setColors	 (colors);
	
    
    // assign a material to the geometry to make it visible. 
    geo->setMaterial(OSG::getDefaultUnlitMaterial());   
	geonode->setCore(geo);
	geogroup->addChild(geonode);


	//the line
	OSG::NodeRefPtr geonode1 = OSG::Node::create();
	OSG::GeometryRefPtr geo1 = OSG::Geometry::create();
	OSG::GeoUInt8PropertyRefPtr type1 = OSG::GeoUInt8Property::create();
	type1->addValue(GL_LINE);
	OSG::GeoUInt32PropertyRefPtr lens1 = OSG::GeoUInt32Property::create();
	lens1->addValue(4);
	OSG::GeoPnt3fPropertyRefPtr  pnts1 = OSG::GeoPnt3fProperty::create();
	//calculate an arrow
	//get a point along the vector
	OSG::Pnt3f pointalongline = getPointAlongLine(ipoint, fpoint,0.8);
	//get the distance to the second point of the vector
	OSG::Real32 dist= fpoint.dist(pointalongline);
	//now look for the points for the arrows
	float angle = 20;
	OSG::Real32 npx = fpoint.x()+(pointalongline.x()-fpoint.x())*cos(OSG::osgDegree2Rad(angle))-(pointalongline.y()-fpoint.y())*sin(OSG::osgDegree2Rad(angle));
	OSG::Real32 npy = fpoint.y()+(pointalongline.x()-fpoint.x())*sin(OSG::osgDegree2Rad(angle))+(pointalongline.y()-fpoint.y())*cos(OSG::osgDegree2Rad(angle));
	pnts1->addValue(OSG::Pnt3f(npx,npy,pointalongline.z()));
	pnts1->addValue(fpoint);
	
	OSG::Real32 npx1 = fpoint.x()+(pointalongline.x()-fpoint.x())*cos(OSG::osgDegree2Rad(-angle))-(pointalongline.y()-fpoint.y())*sin(OSG::osgDegree2Rad(-angle));
	OSG::Real32 npy1 = fpoint.y()+(pointalongline.x()-fpoint.x())*sin(OSG::osgDegree2Rad(-angle))+(pointalongline.y()-fpoint.y())*cos(OSG::osgDegree2Rad(-angle));
	pnts1->addValue(fpoint);
	pnts1->addValue(OSG::Pnt3f(npx1,npy1,pointalongline.z()));

	OSG::GeoVec3fPropertyRefPtr colors1 = OSG::GeoVec3fProperty::create();
	colors1->addValue(color);
	
	geo1->setTypes    (type1);
	geo1->setLengths  (lens1);
	geo1->setPositions(pnts1);
	geo1->setColors	 (colors1);
	
    
    // assign a material to the geometry to make it visible. 
    geo1->setMaterial(OSG::getDefaultUnlitMaterial());   
	geonode1->setCore(geo1);
	geogroup->addChild(geonode1);

	return geogroup;
}
void displayVectorValues(OSG::Vec3f vector){
		std::cout << "Vector values (" << vector.x() << ", " << vector.y() << ", " << vector.z() << ") " << std::endl;
}
