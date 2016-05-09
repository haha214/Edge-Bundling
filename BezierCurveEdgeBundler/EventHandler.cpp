#include <osg/Node>
#include "EventHandler.h"
#include "Graph.h"



UseEventHandler::UseEventHandler(osg::Node* node)
	{
		mGraph = dynamic_cast<Graph*>(node);
		mBoolRoadList = false;
		mBoolBezierCurveList = false;
		mBoolColorBezierCurveList = false;
	}

bool UseEventHandler::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
	
		if (!mGraph)
		{
			return false;
		}

		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer)
		{
			return false;
		}
		switch(ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
			{
				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Q)//°´ÏÂQ¼ü
				{
					if (mBoolRoadList==false)
					{
						mBoolRoadList = mGraph->SetRoadListVisible(0);//Òþ²Ø

					} 
					else
					{
						mBoolRoadList = mGraph->SetRoadListVisible(1);//ÏÔÊ¾
					}
				}
				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_W)
				{
					if (mBoolBezierCurveList==false)
					{
						mBoolBezierCurveList = mGraph->SetBezierCurveListVisible(0);

					} 
					else
					{
						mBoolBezierCurveList = mGraph->SetBezierCurveListVisible(1);
					}
				}
				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_E)
				{
					if (mBoolColorBezierCurveList==false)
					{
						mBoolColorBezierCurveList = mGraph->SetColorBezierCurveListVisible(0);

					} 
					else
					{
						mBoolColorBezierCurveList = mGraph->SetColorBezierCurveListVisible(1);
					}
				}
			}
			break;
		default:
			break;

		}
		return false;
	


}
