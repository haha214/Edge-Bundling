#include "Graph.h"
#include <osgViewer/Viewer>
#include <Windows.h>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/MapNode>
#include <osgEarthUtil//SkyNode>
#include <osgGA/AnimationPathManipulator>
#include <osgViewer/ViewerEventHandlers>
#include "EventHandler.h"


extern HANDLE hMutex=INVALID_HANDLE_VALUE;//全局变量定义时要初始化
int main()
{
	
	//hMutex = CreateMutex(NULL,false,NULL);
	 hMutex = CreateMutex(NULL, FALSE,/* LPCWSTR*//*("Mutex")*/NULL);
	Graph* graph = new Graph;
	graph->Create();
	osg::Group* root = new osg::Group;
    osgViewer::Viewer viewer;
	osg::Node* earth = osgDB::readNodeFile("D:\\OSG\\OSGEarth\\feature_overlay_polysedge.earth");
	root->addChild(earth);

	osgEarth::MapNode* mapNode = osgEarth::MapNode::findMapNode(earth);
	osgEarth::Util::SkyNode* mSky = new osgEarth::Util::SkyNode(mapNode->getMap());
	mSky->setDateTime(2012,3,9,4);
	mSky->attach(&viewer);
	root->addChild(mSky);

	root->addChild(graph);
	root->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	osg::DisplaySettings* ds = osg::DisplaySettings::instance();//抗锯齿
	ds->setNumMultiSamples(4);

	

	viewer.setDisplaySettings(ds);

	viewer.setSceneData(root);
	viewer.setCameraManipulator(new osgEarth::Util::EarthManipulator);
	viewer.addEventHandler(new UseEventHandler(graph));
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);
//	viewer.getCamera()->setClearColor(osg::Vec4(1.0,1.0,1.0,1.0));
	viewer.setUpViewInWindow(0,0,800,800);//使用窗口模式
	viewer.run();
}