#pragma once

#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/Point>
#include <osg/AnimationPath>
#include <osgText/Text>
#include <osg/LineWidth>
#include <sstream>
#include <osg/Group>

class PickHandler:public osgGA::GUIEventHandler
{
public:
	PickHandler();
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	//取点的主要函数
	virtual void pick(osgViewer::Viewer* viewer,const osgGA::GUIEventAdapter& ea);
	//画第一个点图标为一个正方形
	osg::Group* DrawFirstNode(osg::Vec3f position);
	//画其他点为自定义图形
	osg::Node* DrawCtrlPoints(osg::Vec3f position);

	//由于场景过大，可能需要把图标放大相应比例
	int SelectSize(int s);
	//是否开始取点
	void setPick(bool pick);
	bool getPick();

	//根据所取的点，形成路径
	osg::AnimationPath* CreatePath(std::string pathName);
	//得到路径总长度
	double GetAllDistance();
	//得到运行时时间
	float GetRunTime(osg::Vec3 res,osg::Vec3 des);
	//设置每米多少秒走完
	void setSecondsPerMeter(double s);
	//得到设置的值
	double getSecondsPerMeter();


	//创建一条曲线来说明路径
	osg::Node* CreateCardinal(void);
	void Cardlinal(osg::ref_ptr<osg::Vec3Array>temp);
	float GetCoefficient(float p0,float p1,float p2,float p3,float t);


	//使用曲线
	void pushCardinal();


	//得到所有的控制点组结点
	osg::Group* getCtrlPointsGroup();

	//是否开启pick
	bool m_bPick;
	//是否是第一次点击，用来判断是否应该画正方形
	bool m_bFirstPush;
	osg::ref_ptr<osg::Vec3Array> point;
	osg::ref_ptr<osg::Group> ctrlPoints;
	int size;
	double sec;
	osgViewer::Viewer* mv;


};