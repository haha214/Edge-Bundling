#include "PickHandler.h"

PickHandler::PickHandler():
m_bPick(false)
,m_bFirstPush(false)
,size(1.0)
,sec(0.9)
{
	//Ϊ���йؼ������ʱ��
	point = new osg::Vec3Array;

};
void PickHandler::setSecondsPerMeter(double s)
{
	//����ÿ���ö��������꣬Ĭ����0.9��
	sec = s;
}
//�õ�ÿ���ö���������
double PickHandler::getSecondsPerMeter()
{
	return sec;
}
//handle
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	//�ж�ʱ�����ͣ�������Ҫ������ȡ��
	switch(ea.getEventType())
	{
		//����ǵ���
	case (osgGA::GUIEventAdapter::PUSH):
		{
			//���pick�Ѿ�������
			if (m_bPick)
			{
				//��viewer��Ч��ǰ���£�����viewer������pick
				mv = dynamic_cast<osgViewer::Viewer*>(&aa);
				if(mv)pick(mv,ea);
				return false;
			}
		}
	default:
		return false;
	}
}
//�ؼ�pick
void PickHandler::pick(osgViewer::Viewer* viewer,const osgGA::GUIEventAdapter& ea)
{
	//�����
	osgUtil::LineSegmentIntersector::Intersections intersections;
	//�õ������������ݸ��ڵ�
	osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
	//�������򷵻�
	if(!root)return;
	//�����ཻ
	if (viewer->computeIntersections(ea.getX(),ea.getY(),intersections))
	{
		//ȡ������е�ֵ
		osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
		//�õ��ཻ����������ֵ
		osg::Vec3f temp(hitr->getWorldIntersectPoint());
		//����ǵ�һ�ε�������ֻ�غ���һ�ε�ͼ�λ��Ĳ�ͬ����������ͬ
		if (!m_bFirstPush)
		{
			//�ѹؼ��㶼���
			point->clear();
			//ѹ��
			point->push_back(osg::Vec3f(temp.x(),temp.y(),temp.z()));
			//����һ�����Ƶ�
			ctrlPoints = DrawFirstNode(temp);
			//�ѻ��Ŀ��Ƶ����viewer����ʾ����
			root->addChild(ctrlPoints.get());
			//��һ�ε���ر�
			m_bFirstPush = true;
		} 
		else
		{
			//������ǵ�һ�ε������һ����ɫͼ����뵽������
			point->push_back(osg::Vec3f(temp.x(),temp.y(),temp.z()));
			//�������ͼ�굽�����н��л���
			ctrlPoints->addChild(DrawCtrlPoints(temp));
		}
	}
}
//����һ���ڵ㣬Ҳ���ǻ�һ��������
osg::Group* PickHandler::DrawFirstNode(osg::Vec3f position)
{
	osg::Group* root = new osg::Group();

	//���ü��ν��
	osg::Geode* pyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();

	//�����ν�����
	pyramidGeode->addDrawable(pyramidGeometry);
	root->addChild(pyramidGeode);
	//���������ε��ĵ�
	osg::ref_ptr<osg::Vec3Array>trangle = new osg::Vec3Array;
	trangle->push_back(osg::Vec3(position.x()-4.0*size,position.y(),position.z()));
	trangle->push_back(osg::Vec3(position.x(),position.y()-4.0*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()+4.0*size,position.y(),position.z()));
	trangle->push_back(osg::Vec3(position.x(),position.y()+4.0*size,position.z()));

	//�ѵ���뻭��
	pyramidGeometry->setVertexArray(trangle.get());

	//����������ʽ
	osg::DrawElementsUInt* pyramidBase = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_LOOP,0);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	pyramidGeometry->addPrimitiveSet(pyramidBase);

	//��ɫ����
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));

	//������ɫ
	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	return root;

}

//������ͼ����ɫͼ��
osg::Node* PickHandler::DrawCtrlPoints(osg::Vec3f position)
{
	osg::Group* root = new osg::Group();
	//���õ�Ĵ�С
	osg::ref_ptr<osg::Point>pointsize = new osg::Point;
	//���õ��
	pointsize->setSize(5.0);
	root->getOrCreateStateSet()->setAttributeAndModes(pointsize.get(),osg::StateAttribute::ON);

	//���ü��ν��
	osg::Geode* pyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();

	//�ɻ����ν�����
	pyramidGeode->addDrawable(pyramidGeometry);
	root->addChild(pyramidGeode);
	//ѹ��ͼ�궥��ֵ
	osg::ref_ptr<osg::Vec3Array>trangle = new osg::Vec3Array;
	trangle->push_back(osg::Vec3(position.x()-2*size,position.y(),position.z()));
	trangle->push_back(osg::Vec3(position.x()-0.5*size,position.y()-1.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()-0.5*size,position.y()-3.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x(),position.y()-4.0*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()+0.5*size,position.y()-3.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()+0.5*size,position.y()-1.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()+2.0*size,position.y(),position.z()));
	trangle->push_back(osg::Vec3(position.x()+0.5*size,position.y()+1.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()+0.5*size,position.y()+3.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x(),position.y()+4.0*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()-0.5*size,position.y()+3.5*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()-0.5*size,position.y()+1.5*size,position.z()));

	//�ѵ���뻭��
	pyramidGeometry->setVertexArray(trangle.get());

	//����������ʽ
	osg::DrawElementsUInt* pyramidBase = new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON,0);
	pyramidBase->push_back(11);
	pyramidBase->push_back(10);
	pyramidBase->push_back(9);
	pyramidBase->push_back(8);
	pyramidBase->push_back(7);
	pyramidBase->push_back(6);
	pyramidBase->push_back(5);
	pyramidBase->push_back(4);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	pyramidGeometry->addPrimitiveSet(pyramidBase);

	//��ɫ����
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	//������ɫ
	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	return(osg::Node*)root;
}

//����ͼ������ű������еĳ��������ߺܴ�
int PickHandler::SelectSize(int s)
{
	size = 10*s;
	return size;
}

//�����Ƿ���pick
void PickHandler::setPick(bool pick)
{
	m_bPick = pick;
}
//�õ��Ƿ��Ѿ�����pick
bool PickHandler::getPick()
{
	return m_bPick;
}
//��������ȡ�Ĺؼ����ֵ������һ��·���ļ����ļ���ΪpathName.path������һ��ֱ�ӿ��õ�path
osg::AnimationPath* PickHandler::CreatePath(std::string pathName)
{
	//����·������ѭ��ʱ�䣬���ܳ��ȳ˵�λ������ʱ��õ�
	double looptime = sec * GetAllDistance();
	//����Ĵ���Ϊ�����������Ǹ���ʽ������Ҫ������еĿ��Ƶ���Ƹ����ߣ����������һͷһβ�������Ƶ�
	//����Ĵ����Ϊ�������������Ƶ�
	std::vector<osg::Vec3>::iterator iter = point->end();
	std::vector<osg::Vec3>::iterator iter2;
	iter--;
	iter2 = --iter;
	iter++;
	point->push_back(osg::Vec3((*iter).x()-(*iter2).x(),(*iter).y()-(*iter2).y(),(*iter).z()-(*iter2).z()));
	//path�����ظ�path
	osg::AnimationPath* animationPath = new osg::AnimationPath;
	//����pathѭ��ģʽ
	animationPath->setLoopMode(osg::AnimationPath::LOOP);
	//�����ж��ٸ��ؼ���
	int numSamples = point->size();
	//��������������û��ʲô�ã�Ϊ��Щ����׼���ģ���Щ��������������в�û��ʹ��
	float yaw = 0.0f;
	float yaw_delta = 0.5;
	float roll = osg::inDegrees(90.0f);

	double time = 0.0f;
	double time_delta = looptime/(double)numSamples;

	float angle = 0.0;
	iter = point->begin();
	//��ʼ�����������
	for (int i=1;i<numSamples;++i,iter++)
	{

		osg::Vec3 position(*iter);
		iter++;
		if (iter!=point->end())
		{
			//��ֻ��һ�������Ҫ�жϳ���ֻ��Ҫ�ж���������x��нǾͿ����ˣ�����ܶ�
			if ((*iter).x()>position.x())
			{
				angle = 1.57-atan(((*iter).y() - position.y())/((*iter).x() - position.x()));
				if (angle<0)
				{
					angle = 1.57+angle;
				}
			}
			//����һ�����
			if ((*iter).x()<position.x())
			{
				angle = -(1.57+atan(((*iter).y() - position.y())/((*iter).x() - position.x())));
				if (angle>0)
				{
					angle = -(1.57 - angle);
				}
			}
		}
		//�����ž�Ҫ������תʲô����
		osg::Quat rotation(osg::Quat(roll,osg::Vec3(1.0,0.0,0.0))*osg::Quat(-angle,osg::Vec3(0.0,0.0,1.0)));
		osg::Quat rotationY(osg::Quat(-(3.1415926/6.0),osg::Vec3(1.0,0.0,0.0)));

		//���һ������ʼ���룬һ��һ�����
		animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));
		time+=GetRunTime(position,*iter);
		iter--;
	}
	//��·��������ļ�����
	std::ofstream fout(pathName.c_str());
	animationPath->write(fout);
	fout.close();
	//������ʵ����·��
	return animationPath;
}

//�õ�ѹ����·������
double PickHandler::GetAllDistance()
{
	float distant = 0.0;

	osg::Vec3Array::iterator iter = point->begin();

	const int size = point->size();
	if (size<=1)
	{
		return 0;
	} 
	else
	{
		for (int i=0;i<size-1;i++,iter++)
		{
			osg::Vec3 temp = *iter;
			iter++;
			//�����ľ��빫ʽ
			distant+=sqrt((temp.x()-(*iter).x())*(temp.x()-(*iter).x()) + (temp.y()-(*iter).y())*(temp.y()-(*iter).y()));
			iter--;
		}
	}
	return distant;


}
//�õ����������е�ʱ��
float PickHandler::GetRunTime(osg::Vec3 res,osg::Vec3 des)
{

}