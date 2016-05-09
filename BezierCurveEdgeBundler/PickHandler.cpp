#include "PickHandler.h"

PickHandler::PickHandler():
m_bPick(false)
,m_bFirstPush(false)
,size(1.0)
,sec(0.9)
{
	//为所有关键点分配时间
	point = new osg::Vec3Array;

};
void PickHandler::setSecondsPerMeter(double s)
{
	//设置每米用多少秒走完，默认是0.9秒
	sec = s;
}
//得到每米用多少秒走完
double PickHandler::getSecondsPerMeter()
{
	return sec;
}
//handle
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	//判断时间类型，这里主要处理单击取点
	switch(ea.getEventType())
	{
		//如果是单击
	case (osgGA::GUIEventAdapter::PUSH):
		{
			//如果pick已经开启了
			if (m_bPick)
			{
				//在viewer有效的前提下，传人viewer，开启pick
				mv = dynamic_cast<osgViewer::Viewer*>(&aa);
				if(mv)pick(mv,ea);
				return false;
			}
		}
	default:
		return false;
	}
}
//关键pick
void PickHandler::pick(osgViewer::Viewer* viewer,const osgGA::GUIEventAdapter& ea)
{
	//结果集
	osgUtil::LineSegmentIntersector::Intersections intersections;
	//得到场景所有数据根节点
	osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
	//无数据则返回
	if(!root)return;
	//计算相交
	if (viewer->computeIntersections(ea.getX(),ea.getY(),intersections))
	{
		//取结果集中的值
		osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
		//得到相交的世界坐标值
		osg::Vec3f temp(hitr->getWorldIntersectPoint());
		//如果是第一次点击，点击只关乎第一次的图形画的不同，其他都相同
		if (!m_bFirstPush)
		{
			//把关键点都清空
			point->clear();
			//压入
			point->push_back(osg::Vec3f(temp.x(),temp.y(),temp.z()));
			//画第一个控制点
			ctrlPoints = DrawFirstNode(temp);
			//把画的控制点加入viewer中显示出来
			root->addChild(ctrlPoints.get());
			//第一次点击关闭
			m_bFirstPush = true;
		} 
		else
		{
			//如果不是第一次点击，则画一个蓝色图标加入到场景中
			point->push_back(osg::Vec3f(temp.x(),temp.y(),temp.z()));
			//添加所画图标到场景中进行绘制
			ctrlPoints->addChild(DrawCtrlPoints(temp));
		}
	}
}
//画第一个节点，也就是画一个正方形
osg::Group* PickHandler::DrawFirstNode(osg::Vec3f position)
{
	osg::Group* root = new osg::Group();

	//设置几何结点
	osg::Geode* pyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();

	//画几何结点加入
	pyramidGeode->addDrawable(pyramidGeometry);
	root->addChild(pyramidGeode);
	//画入正方形的四点
	osg::ref_ptr<osg::Vec3Array>trangle = new osg::Vec3Array;
	trangle->push_back(osg::Vec3(position.x()-4.0*size,position.y(),position.z()));
	trangle->push_back(osg::Vec3(position.x(),position.y()-4.0*size,position.z()));
	trangle->push_back(osg::Vec3(position.x()+4.0*size,position.y(),position.z()));
	trangle->push_back(osg::Vec3(position.x(),position.y()+4.0*size,position.z()));

	//把点充入画区
	pyramidGeometry->setVertexArray(trangle.get());

	//设置所画格式
	osg::DrawElementsUInt* pyramidBase = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_LOOP,0);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	pyramidGeometry->addPrimitiveSet(pyramidBase);

	//颜色设置
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));

	//设置颜色
	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	return root;

}

//画控制图标蓝色图标
osg::Node* PickHandler::DrawCtrlPoints(osg::Vec3f position)
{
	osg::Group* root = new osg::Group();
	//设置点的大小
	osg::ref_ptr<osg::Point>pointsize = new osg::Point;
	//设置点大
	pointsize->setSize(5.0);
	root->getOrCreateStateSet()->setAttributeAndModes(pointsize.get(),osg::StateAttribute::ON);

	//设置几何结点
	osg::Geode* pyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();

	//可画几何结点加入
	pyramidGeode->addDrawable(pyramidGeometry);
	root->addChild(pyramidGeode);
	//压入图标顶点值
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

	//把点充入画区
	pyramidGeometry->setVertexArray(trangle.get());

	//设置所画格式
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

	//颜色设置
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	//设置颜色
	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	return(osg::Node*)root;
}

//设置图标的缩放比例，有的场景比例尺很大
int PickHandler::SelectSize(int s)
{
	size = 10*s;
	return size;
}

//设置是否开启pick
void PickHandler::setPick(bool pick)
{
	m_bPick = pick;
}
//得到是否已经开启pick
bool PickHandler::getPick()
{
	return m_bPick;
}
//根据所提取的关键点的值来生成一个路径文件，文件名为pathName.path，返回一个直接可用的path
osg::AnimationPath* PickHandler::CreatePath(std::string pathName)
{
	//设置路径的总循环时间，由总长度乘单位米所用时间得到
	double looptime = sec * GetAllDistance();
	//下面的代码为：根据上面那个公式，发现要想过所有的控制点绘制该曲线，必须另加入一头一尾两个控制点
	//下面的代码就为加入者两个控制点
	std::vector<osg::Vec3>::iterator iter = point->end();
	std::vector<osg::Vec3>::iterator iter2;
	iter--;
	iter2 = --iter;
	iter++;
	point->push_back(osg::Vec3((*iter).x()-(*iter2).x(),(*iter).y()-(*iter2).y(),(*iter).z()-(*iter2).z()));
	//path，返回该path
	osg::AnimationPath* animationPath = new osg::AnimationPath;
	//设置path循环模式
	animationPath->setLoopMode(osg::AnimationPath::LOOP);
	//设置有多少个关键点
	int numSamples = point->size();
	//这两个变量好像没有什么用，为有些操作准备的，这些操作在这个程序中并没有使用
	float yaw = 0.0f;
	float yaw_delta = 0.5;
	float roll = osg::inDegrees(90.0f);

	double time = 0.0f;
	double time_delta = looptime/(double)numSamples;

	float angle = 0.0;
	iter = point->begin();
	//开始处理各个点了
	for (int i=1;i<numSamples;++i,iter++)
	{

		osg::Vec3 position(*iter);
		iter++;
		if (iter!=point->end())
		{
			//这只是一种情况，要判断朝向只需要判断两个点与x轴夹角就可以了，情况很多
			if ((*iter).x()>position.x())
			{
				angle = 1.57-atan(((*iter).y() - position.y())/((*iter).x() - position.x()));
				if (angle<0)
				{
					angle = 1.57+angle;
				}
			}
			//另外一种情况
			if ((*iter).x()<position.x())
			{
				angle = -(1.57+atan(((*iter).y() - position.y())/((*iter).x() - position.x())));
				if (angle>0)
				{
					angle = -(1.57 - angle);
				}
			}
		}
		//紧接着就要处理旋转什么的了
		osg::Quat rotation(osg::Quat(roll,osg::Vec3(1.0,0.0,0.0))*osg::Quat(-angle,osg::Vec3(0.0,0.0,1.0)));
		osg::Quat rotationY(osg::Quat(-(3.1415926/6.0),osg::Vec3(1.0,0.0,0.0)));

		//最后一步，开始插入，一点一点插入
		animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));
		time+=GetRunTime(position,*iter);
		iter--;
	}
	//把路径输出到文件当中
	std::ofstream fout(pathName.c_str());
	animationPath->write(fout);
	fout.close();
	//返回真实可用路径
	return animationPath;
}

//得到压入点的路径长度
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
			//两点间的距离公式
			distant+=sqrt((temp.x()-(*iter).x())*(temp.x()-(*iter).x()) + (temp.y()-(*iter).y())*(temp.y()-(*iter).y()));
			iter--;
		}
	}
	return distant;


}
//得到两点间的运行的时间
float PickHandler::GetRunTime(osg::Vec3 res,osg::Vec3 des)
{

}