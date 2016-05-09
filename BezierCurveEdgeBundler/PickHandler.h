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
	//ȡ�����Ҫ����
	virtual void pick(osgViewer::Viewer* viewer,const osgGA::GUIEventAdapter& ea);
	//����һ����ͼ��Ϊһ��������
	osg::Group* DrawFirstNode(osg::Vec3f position);
	//��������Ϊ�Զ���ͼ��
	osg::Node* DrawCtrlPoints(osg::Vec3f position);

	//���ڳ������󣬿�����Ҫ��ͼ��Ŵ���Ӧ����
	int SelectSize(int s);
	//�Ƿ�ʼȡ��
	void setPick(bool pick);
	bool getPick();

	//������ȡ�ĵ㣬�γ�·��
	osg::AnimationPath* CreatePath(std::string pathName);
	//�õ�·���ܳ���
	double GetAllDistance();
	//�õ�����ʱʱ��
	float GetRunTime(osg::Vec3 res,osg::Vec3 des);
	//����ÿ�׶���������
	void setSecondsPerMeter(double s);
	//�õ����õ�ֵ
	double getSecondsPerMeter();


	//����һ��������˵��·��
	osg::Node* CreateCardinal(void);
	void Cardlinal(osg::ref_ptr<osg::Vec3Array>temp);
	float GetCoefficient(float p0,float p1,float p2,float p3,float t);


	//ʹ������
	void pushCardinal();


	//�õ����еĿ��Ƶ�����
	osg::Group* getCtrlPointsGroup();

	//�Ƿ���pick
	bool m_bPick;
	//�Ƿ��ǵ�һ�ε���������ж��Ƿ�Ӧ�û�������
	bool m_bFirstPush;
	osg::ref_ptr<osg::Vec3Array> point;
	osg::ref_ptr<osg::Group> ctrlPoints;
	int size;
	double sec;
	osgViewer::Viewer* mv;


};