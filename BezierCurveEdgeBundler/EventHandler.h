#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include "Graph.h"
#include <osgViewer/Viewer>


class UseEventHandler:public osgGA::GUIEventHandler
{
public:
	UseEventHandler(osg::Node* node);
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
protected:
	Graph* mGraph;
	bool mBoolRoadList;
	bool mBoolBezierCurveList;
	bool mBoolColorBezierCurveList;
};
