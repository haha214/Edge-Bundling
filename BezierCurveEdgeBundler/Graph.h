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
	int start;             //��������,kind�����˱����ĺ��壬kind = STARTʱ�˱���������ʼ������������������ʼ·������
	int end;               //kind = ENDʱ�˱���������ֹ������������������ʼ·������

	float statrtex;     //�˶α�����������ʼ�����������
	float endtex;       //��ֹ�����������

	union
	{
		float startRoadRatio;  //��ʼ���λ����·�ϵı���
		float startProjectPtRatio;
	};
	
	union
	{
		float endRoadRatio;    //��ֹ���λ����·�ϵı���
		float endProjectPtRatio; 
	};
	
	int middleCtrlPt;      //�м���Ƶ��������kindΪSTARTʱ������ߵ���ʼ��ͶӰ��ΪENDʱ������ֹ���ͶӰ�������������·�Ľ���
	std::vector<Point> interPoint;
	float value;
};

struct Road
{
	int startPoint;
	int endPoint;
	float value;
	std::vector<int> bezierStart;     //�Ӵ�·�������ı���������

	std::vector<int> edgePassed;  //ͨ������·�ıߵ�����
	//std::vector<int> firstnum;   //ͨ���ĵ�һ��·�ǵ�ǰ·���ıߵ����
	//std::vector<int> endnum;     //ͨ�������һ��·�ǵ�ǰ·���ıߵ����
	//int fbound;            //����ͨ���뷴��ͨ���ķֽ��±꣨��һ��·������£�
	//int ebound;            //���һ��·�������
};

struct Edge
{
	int startPoint;
	int endPoint;
	float startProjPoint;             //��ʼ����·�ϵ�ͶӰ��
	float endProjPoint;

	std::vector<int> roadPassed;    //������·������

	std::vector<int> bezierNum;     //�˱ߵı��������ߵļ���

};

struct RedBezierCurve   //edge�ṹ���м�¼��ÿ���߱���֮��ı���������ÿ����Ӧ���������ڻ���ͨ��ĳ��·�����бߵ���������������ʱ���ú�ɫ�������ô˽ṹ���¼��Ҫ���Ƶı������߶ε�������Ȩֵ
{
	int BezierCurveNum;   //��ɫ���ߵı�������������

	float value;

};

struct ColorBezierCurve
{
	int BezierCurveNum; //����ɫ���ߵı�������������
	float value;
};

struct XYline{   // Ax+By+C=0;

	float A;

	float B;

	float C;

	int RoadStart;//�߶���ʼ��������±�

	int RoadEnd;//�߶���ֹ��������±�
};

struct PolyLine
{
	Point s;
	Point e;
};

struct RoadValue
{
	float roadvalue;//·��Ȩ��

	int roadnum;//·����ţ�������ʾ����ͨ����������ʾ����ͨ��
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
	bool SetEdgePointsVisible();//���ñߵĶ�����ʾ״̬
	bool SetRoadListVisible(int i);
	bool SetBezierCurveListVisible(int i);
	bool SetColorBezierCurveListVisible(int i);
private:
	
	XYline MakeLine(int startnum,int endnum);
	float dis_PL(Point P,XYline line); //�㵽ֱ�߾���
	float dis_PP(Point p1,Point p2);//��������
	Point ProjectPointToLine(Point s,Point e,Point p);//�㵽ֱ�ߴ����λ��
	float dis_PS(Point p,XYline line);
	float Per(Point p,XYline line);
	void Judgment(Point p,XYline line,int& Index,int& s1num,int& s2num);
	void Sdis_PL(Point P,std::vector<XYline> line,int& Index,int& s1num,int& s2num,int& linenum);
	void Dijkstra(int n,std::vector<std::vector<RoadValue>> map,int s1,int s2,int e1,int e2,int index1,int index2,Edge& edge,int edgenum);
    void ComputeShortestPath(int EdgeStart,int EdgeEnd);

	void ThreadComputeSP();
	void SetThreadNum(int n);

	void ReadShp(std::string ShpFileName,std::vector<PolyLine>& PL);//����.shp�ļ�������·
	int SamePoint(Point p1,Point p2);  //�ж��Ƿ�Ϊ��ͬ��
	void MakeRoad(std::vector<PolyLine> PL);//����·
	void RshpMroad(std::string ShpFileName);

	Point MakeVector (Point p1,Point p2); //�������һ����
	float Cos(Point v1,Point v2); //������������нǵ�cosֵ
	void AngleMeasure(Edge e,float factor); //���н��жϲ�����·�ĺϲ�

	//void FERoad(Edge e,Road r,int num); //��¼һ���ߵ����·���еĵ�һ��·�����һ��·
	//void SortEdge(Road r);

	bool initPointsFromXML(TiXmlElement* graphEle,int n);
	bool initEdgeFromXML(TiXmlElement* graphEle,int n,int EdgePointNum);
	bool initGraphInfo(int EdgePointNum,int EdgeNum);




	float Factorial(int n);
	float Bernstein(int n,int k,float t);
	void ComputeBezierInterPoint();
	void CreateOneBezier(std::vector<Point>& interPtArray,Point* ctrlPt,int interPtNum);
	void CreateBezierCurveFromEdgeInfo(Edge& edgeInfo);

	void CreateRedBezierCurveFromRoadInfo(Road roadInfo,int direction);//ѡ��һ��·����ȡ����ͨ������·�ıߣ�����Щ�������ı��������߸���,����ѡ��˫����ı߷ֱ���ʾ

	void ComputeBezierTex(Road roadInfo,int direction);//ѡ��һ��·����ȡ����ͨ������·�ıߣ�����Щ�������ı����������ý���ɫ��ʾ������,����ѡ��˫����ı߷ֱ���ʾ

	void ShaderCreate();


	osg::Geode* CreateBezierGeom();
	osg::Geode* CreateRoadGeom();

	osg::Geode* CreateEdgeGeom();

	osg::Geode* CreateALLEdgeGeom();

	osg::Geode* CreateAllEdgePoint();

	osg::Geode* CreateRedBezierGeom();//���Ƹ����ı���������

	osg::Geode* CreateColorBezierGeom();//���Ƹ����Ľ���ɫ����������

	bool Init();
	bool MakeFakeData(int EdgePointNum,int EdgeNum);
	void CreateBezierCurveWithoutBundler();
	void CreateBezierCurveWithCluster();
	float ComputeBezierInRoadRatio(int road,float projPointRatio);
	float ComputeDistance(Point &p1,Point &p2);
	Point ComuputeInterPointInRoad(int roadIndex,float ratio);

protected:
	std::vector<Point> mRoadPoint;   //·�ϵ�����  �ṩ
	std::vector<Point> mEdgePoints;   //�ߵĶ�������  �ṩ
//	std::vector<Point> mProjectPoints;   //�ߵ���ʼ�����ֹ����·�ϵ�ͶӰ  ��Ҫ
	std::vector<Road> mRoadList;       //·�ļ���   �ṩ
	std::vector<Edge> mEdgeList;       //�ߵļ���   �ṩ
	std::vector<BezierCurve> mBezierCurveList;	//���������ߵļ���
	std::vector<int> mUnPassEdgeNUM;//������·�ıߵ��±�����

	std::vector<RedBezierCurve> mRedBezierCurveList; //��ɫ���������ߵļ���
	
	std::vector<ColorBezierCurve> mColorBezierCurveList;//����ɫ���������ߵļ���

	int mRoadNum; //��ʼʱ·������
	int mThreadSize;//�߳�����

	osg::Geode* mBezierCurve; // ���Ƶı���������
	osg::Geode* mColorBezierCurve;//���ƵĽ���ɫ����������
	osg::Geode* mRoadCurve;//���ƵĹ�·����
	osg::Geode* mDrawEdgePoints;
private:
	bool mIsInit;
	BundlerKind mBundlerKind;
};