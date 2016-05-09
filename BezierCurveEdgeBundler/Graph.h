#pragma once
#include <osg/Group>
#include <osg/Geode>
#include "tinystr.h"
#include "tinyxml.h"
#include <Windows.h>



class Graph;
enum BezierKind
{
	START = 0,
	MIDDLE = 1,
	END = 2
};

struct Point
{
	float x;
	float y;
};

struct MultiThread
{
	Graph* ptr;
	int startnum;
	int endnum;
};

struct BezierCurve
{
	BezierKind kind;
	int start;             //都是索引,kind决定此变量的含义，kind = START时此变量代表起始点的索引，否则代表起始路的索引
	int end;               //kind = END时此变量代表终止点的索引，否则代表起始路的索引

	float statrtex;     //此段贝塞尔曲线起始点的纹理坐标
	float endtex;       //终止点的纹理坐标

	union
	{
		float startRoadRatio;  //起始点的位置在路上的比例
		float startProjectPtRatio;
	};
	
	union
	{
		float endRoadRatio;    //终止点的位置在路上的比例
		float endProjectPtRatio; 
	};
	
	int middleCtrlPt;      //中间控制点的索引，kind为START时，代表边的起始点投影，为END时代表终止点的投影，否则代表两条路的交点
	std::vector<Point> interPoint;
	float value;
};

struct Road
{
	int startPoint;
	int endPoint;
	float value;
	std::vector<int> bezierStart;     //从此路上引出的贝塞尔曲线

	std::vector<int> edgePassed;  //通过这条路的边的索引
	//std::vector<int> firstnum;   //通过的第一条路是当前路径的边的序号
	//std::vector<int> endnum;     //通过的最后一条路是当前路径的边的序号
	//int fbound;            //正向通过与反向通过的分界下标（第一条路的情况下）
	//int ebound;            //最后一条路的情况下
};

struct Edge
{
	int startPoint;
	int endPoint;
	float startProjPoint;             //起始点在路上的投影点
	float endProjPoint;

	std::vector<int> roadPassed;    //经过的路的索引

	std::vector<int> bezierNum;     //此边的贝塞尔曲线的集合

};

struct RedBezierCurve   //edge结构体中记录了每条边变形之后的贝塞尔曲线每段相应的索引，在绘制通过某条路的所有边的完整贝塞尔曲线时，用红色高亮，用此结构体记录需要绘制的贝塞尔线段的索引和权值
{
	int BezierCurveNum;   //红色曲线的贝塞尔曲线索引

	float value;

};

struct ColorBezierCurve
{
	int BezierCurveNum; //渐变色曲线的贝塞尔曲线索引
	float value;
};

struct XYline{   // Ax+By+C=0;

	float A;

	float B;

	float C;

	int RoadStart;//线段起始点的数组下标

	int RoadEnd;//线段终止点的数组下标
};

struct PolyLine
{
	Point s;
	Point e;
};

struct RoadValue
{
	float roadvalue;//路的权重

	int roadnum;//路的序号，正数表示正向通过，负数表示反向通过
};
enum BundlerKind
{
	BEZIER_ONLY = 0,
	BEZIER_CLUSTER = 1
};

class Graph:public osg::Group
{
public:
	Graph();
	~Graph();
	void Create();
	void SetBundlerKind(BundlerKind bundlerKind)
	{
		mBundlerKind = bundlerKind;
	}
	static void ComputeThread(void* para);
	bool SetEdgePointsVisible();//设置边的顶点显示状态
	bool SetRoadListVisible(int i);
	bool SetBezierCurveListVisible(int i);
	bool SetColorBezierCurveListVisible(int i);
private:
	
	XYline MakeLine(int startnum,int endnum);
	float dis_PL(Point P,XYline line); //点到直线距离
	float dis_PP(Point p1,Point p2);//两点间距离
	Point ProjectPointToLine(Point s,Point e,Point p);//点到直线垂点的位置
	float dis_PS(Point p,XYline line);
	float Per(Point p,XYline line);
	void Judgment(Point p,XYline line,int& Index,int& s1num,int& s2num);
	void Sdis_PL(Point P,std::vector<XYline> line,int& Index,int& s1num,int& s2num,int& linenum);
	void Dijkstra(int n,std::vector<std::vector<RoadValue>> map,int s1,int s2,int e1,int e2,int index1,int index2,Edge& edge,int edgenum);
    void ComputeShortestPath(int EdgeStart,int EdgeEnd);

	void ThreadComputeSP();
	void SetThreadNum(int n);

	void ReadShp(std::string ShpFileName,std::vector<PolyLine>& PL);//读入.shp文件并构建路
	int SamePoint(Point p1,Point p2);  //判断是否为相同点
	void MakeRoad(std::vector<PolyLine> PL);//构建路
	void RshpMroad(std::string ShpFileName);

	Point MakeVector (Point p1,Point p2); //两点产生一向量
	float Cos(Point v1,Point v2); //计算两向量间夹角的cos值
	void AngleMeasure(Edge e,float factor); //进行角判断并进行路的合并

	//void FERoad(Edge e,Road r,int num); //记录一条边的最短路径中的第一条路和最后一条路
	//void SortEdge(Road r);

	bool initPointsFromXML(TiXmlElement* graphEle,int n);
	bool initEdgeFromXML(TiXmlElement* graphEle,int n,int EdgePointNum);
	bool initGraphInfo(int EdgePointNum,int EdgeNum);




	float Factorial(int n);
	float Bernstein(int n,int k,float t);
	void ComputeBezierInterPoint();
	void CreateOneBezier(std::vector<Point>& interPtArray,Point* ctrlPt,int interPtNum);
	void CreateBezierCurveFromEdgeInfo(Edge& edgeInfo);

	void CreateRedBezierCurveFromRoadInfo(Road roadInfo,int direction);//选定一条路，获取所有通过这条路的边，将这些边完整的贝塞尔曲线高亮,可以选择单双方向的边分别显示

	void ComputeBezierTex(Road roadInfo,int direction);//选定一条路，获取所有通过这条路的边，将这些边完整的贝塞尔曲线用渐变色表示方向性,可以选择单双方向的边分别显示

	void ShaderCreate();


	osg::Geode* CreateBezierGeom();
	osg::Geode* CreateRoadGeom();

	osg::Geode* CreateEdgeGeom();

	osg::Geode* CreateALLEdgeGeom();

	osg::Geode* CreateAllEdgePoint();

	osg::Geode* CreateRedBezierGeom();//绘制高亮的贝塞尔曲线

	osg::Geode* CreateColorBezierGeom();//绘制高亮的渐变色贝塞尔曲线

	bool Init();
	bool MakeFakeData(int EdgePointNum,int EdgeNum);
	void CreateBezierCurveWithoutBundler();
	void CreateBezierCurveWithCluster();
	float ComputeBezierInRoadRatio(int road,float projPointRatio);
	float ComputeDistance(Point &p1,Point &p2);
	Point ComuputeInterPointInRoad(int roadIndex,float ratio);

protected:
	std::vector<Point> mRoadPoint;   //路上点数组  提供
	std::vector<Point> mEdgePoints;   //边的顶点数组  提供
//	std::vector<Point> mProjectPoints;   //边的起始点和终止点在路上的投影  需要
	std::vector<Road> mRoadList;       //路的集合   提供
	std::vector<Edge> mEdgeList;       //边的集合   提供
	std::vector<BezierCurve> mBezierCurveList;	//贝塞尔曲线的集合
	std::vector<int> mUnPassEdgeNUM;//不经过路的边的下标序列

	std::vector<RedBezierCurve> mRedBezierCurveList; //红色贝塞尔曲线的集合
	
	std::vector<ColorBezierCurve> mColorBezierCurveList;//渐变色贝塞尔曲线的集合

	int mRoadNum; //初始时路的数量
	int mThreadSize;//线程数量

	osg::Geode* mBezierCurve; // 绘制的贝塞尔曲线
	osg::Geode* mColorBezierCurve;//绘制的渐变色贝塞尔曲线
	osg::Geode* mRoadCurve;//绘制的公路曲线
	osg::Geode* mDrawEdgePoints;
private:
	bool mIsInit;
	BundlerKind mBundlerKind;
};