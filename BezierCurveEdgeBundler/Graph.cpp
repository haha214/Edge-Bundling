#include "Graph.h"
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/Point>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <osgDB/ReadFile>
#include <fstream>


float Graph::Factorial( int n )
{
	float result=1.0;
	if(n==0||n==1)
		return 1.0;	
	for(int i=n;i>1;i--)
	{
		result*=float(i);
	}
	return result;
}

float Graph::Bernstein( int n,int k,float t )
{
	float bt=Factorial(n);

	bt/=Factorial(k);

	bt/=Factorial(n-k);

	if(k!=0)
	{
		bt*=pow(t,k);	
	}	
	if(n-k!=0)
	{
		bt*=pow(1.0-t,n-k);		
	}
	return bt;
}

void Graph::CreateOneBezier( std::vector<Point>& interPtArray,Point* ctrlPt,int interPtNum )
{
	interPtArray.resize(0);
	Point interPt;
	float dt = 1.0/(float)interPtNum;
	for(float i=0.0;i<=interPtNum;i+=1.0)
	{
		interPt.x = 0;
		interPt.y = 0;
		for(int j=0;j<3;j++)
		{
			interPt.x+=Bernstein(2,j,i*dt)*ctrlPt[j].x;
			interPt.y+=Bernstein(2,j,i*dt)*ctrlPt[j].y;		
		}
		interPtArray.push_back(interPt);
	}
}

void Graph::Create()
{
	if (!Init())
	{
		return;
	}
	switch(mBundlerKind)
	{
	case BEZIER_ONLY:
		CreateBezierCurveWithoutBundler();
		break;
	case BEZIER_CLUSTER:
		break;
	}
	ComputeBezierInterPoint();

	std::ofstream fs;
	fs.open("101.txt");
	int interPointNum = 0;
	//for (int i=0;i<mBezierCurveList.size();i++)
	//{
	//	for (int j=0;j<mBezierCurveList[i].interPoint.size();j++)
	//	{
	//		fs<<mBezierCurveList[i].interPoint[j].x;
	//		fs<<" ";
	//		fs<<mBezierCurveList[i].interPoint[j].y;
	//		fs<<" ";
	//		fs<<mBezierCurveList[i].value<<std::endl;
	//		interPointNum++;

	//	}
	//}//���ձ��������߶���
	for (int i=0;i<mEdgeList.size();i++)
	{
		for (int j=0;j<mEdgeList[i].bezierNum.size();j++)
		{
			for (int k=0;k<mBezierCurveList[j].interPoint.size();k++)
			{
				fs<<mBezierCurveList[j].interPoint[k].x;
				fs<<" ";
				fs<<mBezierCurveList[j].interPoint[k].y;
				fs<<" ";
				fs<<i<<std::endl;
				interPointNum++;
			}
		}
	}//����ÿ���߶�ȡ���ɱߵı����������ϵĲ�ֵ��
	fs.close();
	std::cout<<"��ֵ��������ǣ�"<<interPointNum<<std::endl;



	/*addChild(CreateBezierGeom());*/
	/*addChild(CreateEdgeGeom());*/

	/*addChild(CreateRoadGeom());*/
	
	/*addChild(CreateALLEdgeGeom());*/
	/*addChild(CreateAllEdgePoint());*/

	//addChild(CreateRedBezierGeom());
	
	/*addChild(CreateColorBezierGeom());*/
	mBezierCurve = CreateBezierGeom();
	
	mRoadCurve = CreateRoadGeom();
    mColorBezierCurve = CreateColorBezierGeom();
	mDrawEdgePoints = CreateAllEdgePoint();


	mColorBezierCurve->setCullingActive(false);//�ر�osg���Զ��޳�����
	mRoadCurve->setCullingActive(false);
	mDrawEdgePoints->setCullingActive(false);
    mBezierCurve->setCullingActive(false);//�ر�osg���Զ��޳����ܣ����������shader��������任�����ɵ�ͼ�ηǳ�С��λ�ڴ������ڲ���osg���Զ���������ʾ����������޳���ʹ��shader��osg����֪��shader�����������ת�������ɽ����޳�

	mRoadCurve->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);//�ر���ȼ��
	mColorBezierCurve->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	mBezierCurve->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	mDrawEdgePoints->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

	ShaderCreate();

	
}

osg::Geode* Graph::CreateBezierGeom()
{
	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArray = new osg::Vec4Array;
	int lastIndex=0;
	for (int j=0;j<mBezierCurveList.size();j++)
	{
		osg::Geometry* curve = new osg::Geometry;		
		for (int i=0;i<mBezierCurveList[j].interPoint.size();i++)
		{
			pointArray->push_back(osg::Vec4(mBezierCurveList[j].interPoint[i].x,mBezierCurveList[j].interPoint[i].y,0.0,1.0));
		}

		osg::Vec4Array* colorArray = new osg::Vec4Array;
		colorArray->push_back(osg::Vec4(0.7,0.7,0.7,0.5));

		curve->setColorArray(colorArray);
		curve->setColorBinding(osg::Geometry::BIND_OVERALL);
		curve->setVertexArray(pointArray);
		curve->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,lastIndex,mBezierCurveList[j].interPoint.size()));

		lastIndex += mBezierCurveList[j].interPoint.size();
		osg::LineWidth* linewidth = new osg::LineWidth();

		linewidth->setWidth(mBezierCurveList[j].value*0.05+1.5);
		curve->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
		graphGeode->addDrawable(curve);
	}
	return graphGeode;
}
 
bool Graph::MakeFakeData(int EdgePointNum,int EdgeNum)
{
	

	//����·   ���ļ���
	Road rd0;
	mRoadList.push_back(rd0);
	RshpMroad("..\\data\\in15oc03\\in101503.shp");
	mRoadNum = mRoadList.size();
	/*std::cout<<mRoadNum<<std::endl;*/

	//����ߵĵ�  ���ļ���


    if (!initGraphInfo(EdgePointNum,EdgeNum))
    {
		return false;
    }
	//for (int i=0;i<EdgeNum;i++)
	//{
	//	mEdgeList[i].endProjPoint = 0.3;
	//	mEdgeList[i].startProjPoint = 0.4;
	//	mEdgeList[i].roadPassed.push_back(3);
	//	mEdgeList[i].roadPassed.push_back(6);
	//	mEdgeList[i].roadPassed.push_back(10);
	//		mEdgeList[i].roadPassed.push_back(16);
	//			mEdgeList[i].roadPassed.push_back(26);

	//}




	SetThreadNum(16);
	ThreadComputeSP();
	//ComputeShortestPath();
	return true;
	

}

bool Graph::Init()
{
	if (mIsInit)
	{
		return true;
	}
	//if (!MakeFakeData(6516,9779))
	if (!MakeFakeData(6516,9779))
	{
		return false;
	}
	//MakeFakeData();

	return true;
}

Graph::Graph()
{
	mIsInit = false;
	mBundlerKind = BEZIER_ONLY;
}

Graph::~Graph()
{
	/*std::cout<<"ddddddd"<<std::endl;*/

}

void Graph::CreateBezierCurveWithoutBundler()
{
	for(int i=0;i<mEdgeList.size();i++)
	{
		if (mEdgeList[i].roadPassed[0]!=0)
		{
			CreateBezierCurveFromEdgeInfo(mEdgeList[i]);
		}
		
	}
	//CreateRedBezierCurveFromRoadInfo(mRoadList[55],2);//ѡ��ĳ��·������ͨ����·�ϵ����бߵ�ȫ�����������ߣ��ڶ���ϵ��Ϊ�趨����1Ϊ����ͨ��·�ıߣ�2Ϊ����ͨ����3Ϊ˫��ȫ��
	ComputeBezierTex(mRoadList[67],1);
}

void Graph::CreateBezierCurveWithCluster()
{
	
}

//����ÿ���ߵĸ������������ߵĿ��Ƶ���Ϣ
void Graph::CreateBezierCurveFromEdgeInfo( Edge& edgeInfo )
{
	float edgeStartRoadRatio = ComputeBezierInRoadRatio(edgeInfo.roadPassed[0],edgeInfo.startProjPoint);
	float edgeLastRoadRatio = ComputeBezierInRoadRatio(-1*edgeInfo.roadPassed[edgeInfo.roadPassed.size()-1],edgeInfo.endProjPoint);
	int bezierNum = edgeInfo.roadPassed.size()+1;
	BezierCurve bcStart;
	bcStart.kind = START;
	bcStart.start = edgeInfo.startPoint;
	bcStart.end = edgeInfo.roadPassed[0]; 
	bcStart.value = 1.0;	
	bcStart.startProjectPtRatio = edgeInfo.startProjPoint;
	bcStart.endRoadRatio = edgeStartRoadRatio;
	//bcStart.middleCtrlPt = edgeInfo.startProjPoint;
	mBezierCurveList.push_back(bcStart);

	edgeInfo.bezierNum.push_back(mBezierCurveList.size()-1);//�˶α����������Ǳ�edgeInfo���κ��һ���֣���¼�´˶α����������ڱ��������߼����е�����

	float lastRatio = bcStart.endRoadRatio;
	for (int i=0;i<bezierNum-2;i++)
	{
		int j;
		//float endRoadRatio = (i == (bezierNum-3)?edgeLastRoadRatio:0.5);
		int startRoad = edgeInfo.roadPassed[i];
		int endRoad = edgeInfo.roadPassed[i+1];
		float plus_minus = (endRoad>0?0.25:0.75);//������·Ϊ0.2��������·Ϊ0.8
		//float plus_minus = 0.5;
		float endRoadRatio = (i == (bezierNum-3)?edgeLastRoadRatio:plus_minus);
		int roadBeizerNum = mRoadList[abs(startRoad)].bezierStart.size(); //���������������ߵ���ʼ·��ͬ�ı�����������
		for(j=0;j<roadBeizerNum;j++)
		{
			int beizerIndex = mRoadList[abs(startRoad)].bezierStart[j];
			
			if (mBezierCurveList[beizerIndex].kind == MIDDLE && endRoad == mBezierCurveList[beizerIndex].end)
			{
				if (abs(mBezierCurveList[beizerIndex].startRoadRatio - lastRatio)<0.01 && abs(mBezierCurveList[beizerIndex].endRoadRatio - endRoadRatio)<0.01)
				{
					mBezierCurveList[beizerIndex].value += 1.0;
					lastRatio = endRoadRatio;

					edgeInfo.bezierNum.push_back(beizerIndex);

					break;
				}
			}				
		}
		if (j == roadBeizerNum) //����û�ҵ�·��һ���ı��������ߣ�ֻ�����¹���һ��
		{
			BezierCurve bc;
			bc.kind = MIDDLE;
			bc.start = edgeInfo.roadPassed[i];
			bc.end = edgeInfo.roadPassed[i+1]; 
			bc.startRoadRatio = lastRatio;
			bc.endRoadRatio = endRoadRatio;
			lastRatio = bc.endRoadRatio;
			bc.value = 1.0;	
			if (bc.start>0)
			{
				bc.middleCtrlPt = mRoadList[bc.start].endPoint;
			}
			else
				bc.middleCtrlPt = mRoadList[bc.start*-1].startPoint;

			mBezierCurveList.push_back(bc);
			mRoadList[abs(startRoad)].bezierStart.push_back(mBezierCurveList.size()-1);

			edgeInfo.bezierNum.push_back(mBezierCurveList.size()-1);
		}		
	}

	//���һ������������
	BezierCurve bcEnd;
	bcEnd.kind = END;
	bcEnd.start = edgeInfo.roadPassed[bezierNum-2];
	bcEnd.end = edgeInfo.endPoint; 
	bcEnd.startRoadRatio = lastRatio;
	bcEnd.endProjectPtRatio = edgeInfo.endProjPoint;
	//lastRatio = bcEnd.endRoadRatio;
	bcEnd.value = 1.0;	
	bcEnd.middleCtrlPt = edgeInfo.endProjPoint;
	mBezierCurveList.push_back(bcEnd);
	int lastPassRoad = edgeInfo.roadPassed[bezierNum-2];
	mRoadList[abs(lastPassRoad)].bezierStart.push_back(mBezierCurveList.size()-1);

	edgeInfo.bezierNum.push_back(mBezierCurveList.size()-1);
	//bc.endRoadRatio = ComputeProjRatio(bc.endRoad,edgeInfo.startProjPoint);
}


void Graph::CreateRedBezierCurveFromRoadInfo(Road roadInfo,int direction)//ѡ��һ��·����ȡ����ͨ������·�ıߣ�����Щ�������ı��������߸���������ѡ��ͬ����ı߷ֱ���ʾ
{
	int edgeNum = roadInfo.edgePassed.size();//ͨ����ǰ·�����бߵ�������
	for (int i=0;i<edgeNum;i++)
	{
		int BezierNum = 0;//ÿ���߱��κ��������ߵĶ�������ʼ��Ϊ0

		if (direction==1)  //ֻѡ������ͨ��·roadInfo�ı�
		{
			if (roadInfo.edgePassed[i]>0)
			{
				BezierNum = mEdgeList[roadInfo.edgePassed[i]].bezierNum.size();
			}
		}
		if (direction==2) //ֻѡ����ͨ��·�ı�
		{
			if (roadInfo.edgePassed[i]<0)
			{
				BezierNum = mEdgeList[abs(roadInfo.edgePassed[i])].bezierNum.size();
			}

		}
		if (direction==3) //����ͨ��·�ıߣ���������ͨ�����Ƿ���ͨ��
		{
			BezierNum = mEdgeList[abs(roadInfo.edgePassed[i])].bezierNum.size();
		}

		if (BezierNum!=0)//ȷ��ֻ�з��������ı߲��ܲ������
		{
			for (int j=0;j<BezierNum;j++)
			{
				if (mRedBezierCurveList.size()!=0)//���Ҫ�����ĺ�ɫ���������߼��ϲ�Ϊ��
				{
					int k;
					int BCNum = mEdgeList[abs( roadInfo.edgePassed[i])].bezierNum[j];
					for (k=0;k<mRedBezierCurveList.size();k++)      //�ͼ��������еı��������߶ν��бȽϣ������ͬһ�Σ���Ȩ�ؼ�1
					{
						if (BCNum==mRedBezierCurveList[k].BezierCurveNum)
						{
							mRedBezierCurveList[k].value +=1.0;
							break;
						} 

					}
					if (k==mRedBezierCurveList.size())     //����˶α��������߲����ڼ����У���Ϊ�¶Σ�ѹ��vector��
					{
						RedBezierCurve rb ;
						rb.BezierCurveNum = BCNum;
						rb.value = 1.0;
						mRedBezierCurveList.push_back(rb);

					}

				}
				else   //��ʱ����Ϊ�գ�ֱ�ӽ����������߶�ѹ��
				{
					RedBezierCurve rb;
					rb.BezierCurveNum = mEdgeList[abs(roadInfo.edgePassed[i])].bezierNum[j];
					rb.value = 1.0;
					mRedBezierCurveList.push_back(rb);
				}
			}
		}
	    


	}


}

void Graph::ComputeBezierTex(Road roadInfo,int direction)
{
	int edgeNum = roadInfo.edgePassed.size();//ͨ��·roadInfo���еı�
	std::vector<int> edgeNo;//ͨ������ѡ����ѡ������ıߵ����
	int RoadbezierStartNum = roadInfo.bezierStart.size();//�Ӵ�·�������ı��������ߵ�����
	for (int i =0;i<edgeNum;i++)//���з����жϣ��ѷ��������ı߱��浽edgeNo��
	{
		if (direction==1)//����
		{
			if (roadInfo.edgePassed[i]>0)
			{
				edgeNo.push_back(roadInfo.edgePassed[i]);
			}
		}
		if (direction==2)//����
		{
			if (roadInfo.edgePassed[i]<0)
			{
				edgeNo.push_back(abs(roadInfo.edgePassed[i]));
			}
		}
		if (direction==3)//˫��
		{
			edgeNo.push_back(abs(roadInfo.edgePassed[i]));
		}
	}
	int edgeNoSize = edgeNo.size();//��Ҫ������ʾ�ıߵ�����
	if (edgeNoSize!=0) //���������Ϊ0
	{
		for (int j=0;j<edgeNoSize;j++)
		{
			    int edgebeziernum = mEdgeList[edgeNo[j]].bezierNum.size();
				for (int i=0;i<edgebeziernum;i++)
				{
					if (mColorBezierCurveList.size()!=0)//�������ɫ���������߼��ϲ�Ϊ��
					{
					   int k;
					   for ( k=0;k<mColorBezierCurveList.size();k++)//�ͼ��������еĽ���ɫ���������߽��бȽ�
					     {
						    if (mEdgeList[edgeNo[j]].bezierNum[i]==mColorBezierCurveList[k].BezierCurveNum)//�����ͬһ��
						    {
							    mColorBezierCurveList[k].value +=1.0;//Ȩ�ؼ�һ
							    break;
					      	} 
   
					     }
					  if (k==mColorBezierCurveList.size())//��������в����ڴ˶Σ��򴴽�Ϊһ�¶Σ�ѹ��vector��
					   {
						   ColorBezierCurve cb;
						   cb.BezierCurveNum = mEdgeList[edgeNo[j]].bezierNum[i];
						   cb.value = 1.0;
						   mColorBezierCurveList.push_back(cb);
					   }
				    }
					else//����ɫ���������߼���Ϊ��
					{
						ColorBezierCurve cb;//�����¶Σ�ѹ��vector��
						cb.BezierCurveNum = mEdgeList[edgeNo[j]].bezierNum[i];
						cb.value = 1.0;
						mColorBezierCurveList.push_back(cb);

					}
				}



			int k1,k2 ;
			for (k1=0;k1<RoadbezierStartNum;k1++)//ȷ����ǰ�߾�������֮�����������������Ǵ�·roadinfo��������
			{
				for (k2=0;k2<edgebeziernum;k2++)//Ѱ�ҳ���mEdgeList[edgeNo[j]]���κ��Ķα����������Ǵ�·roadinfo��������
				{
					if (roadInfo.bezierStart[k1]==mEdgeList[edgeNo[j]].bezierNum[k2])
					{
						break;
					}
				}
				if (k2<edgebeziernum)
				{
					break;
				}
			}
			BezierCurve roadbezierk1 = mBezierCurveList[roadInfo.bezierStart[k1]];
			mBezierCurveList[roadInfo.bezierStart[k1]].statrtex = 0.5;//��roadInfo.bezierStart[k1]Ϊ���ģ�������������Ϊ0.5
			if (roadbezierk1.kind == END)//��������һ������
			{
				mBezierCurveList[roadInfo.bezierStart[k1]].endtex = 1.0;

			} 
			else
			{
				mBezierCurveList[roadInfo.bezierStart[k1]].endtex = 0.5+(1.0-0.5)/5.0;
			}
			float lastendtex = 0.5;
			int lastendNo = roadbezierk1.start;
			for (int p=0;p<edgebeziernum;p++)//��roadInfo.bezierStart[k1]Ϊ���ģ���ǰ���μ����mEdgeList[edgeNo[j]]��ÿ�α��������ߵ���������
			{
				BezierCurve jp = mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[p]];
				if (jp.end == lastendNo && jp.kind!=END)
				{
					mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[p]].endtex = lastendtex;
					lastendtex = 4.0/5.0*lastendtex;

					
					if (jp.kind == START)//����ǵ�һ������
					{
						mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[p]].statrtex = 0.0;
						//lastendNo = -2;
						break;
					} 
					if(jp.kind == MIDDLE)
					{
						mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[p]].statrtex = lastendtex;
						lastendNo = mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[p]].start;
						p = -1;
					}

				}
			}
			int nextstartNo = roadbezierk1.end;
			float nextstarttex = 0.5+(1.0-0.5)/5.0;
			if (roadbezierk1.kind!=END)//roadbezierk1Ϊ�Ӵ�·�������ı��������ߣ���kind����ΪEND����MIDDLE�����ΪEND���ʾΪ���һ�����ߣ���������
			{
				for (int q=0;q<edgebeziernum;q++)//��roadInfo.bezierStart[k1]Ϊ���ģ�������μ����mEdgeList[edgeNo[j]]��ÿ�α��������ߵ���������
				{
					BezierCurve jq = mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[q]];
					if (jq.start == nextstartNo && jq.kind!=START)
					{
						mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[q]].statrtex = nextstarttex;
						nextstarttex = nextstarttex+(1.0-nextstarttex)/5.0;
						if (jq.kind == END)
						{
							mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[q]].endtex = 1.0;
							break;
						} 
						if(jq.kind == MIDDLE)
						{
							mBezierCurveList[mEdgeList[edgeNo[j]].bezierNum[q]].endtex = nextstarttex;
							nextstartNo = jq.end;
							q = -1;
						}
					}
				}

			}


		}
	}
	if (mColorBezierCurveList.size()==0)
	{
		std::cout<<"kong"<<std::endl;
	}
	
}



float Graph::ComputeBezierInRoadRatio(int road,float projPointRatio)
{
	


		if (road>0)
		{

			if (projPointRatio>=0.0 && projPointRatio<0.4)
			{
				return 0.7;
			}
			if (projPointRatio>=0.4 && projPointRatio<0.7)
			{
				return 0.85;
			}
			if (projPointRatio>=0.7 && projPointRatio<=1.0)
			{
				return 1.0;
			}
			/*return projPointRatio*0.5+0.5;*/ //���н��ķ���Ϊ·������������㹫ʽΪx+��1-x��*0.5
		}
		else
		{
			/*return projPointRatio*0.5;*/ //����ֱ��ΪͶӰ������һ��
	
			if (projPointRatio>=0 && projPointRatio<=0.3)
			{
				return 0.0;
			}
			if (projPointRatio>0.3 && projPointRatio<=0.6)
			{
				return 0.15;
			}
			if (projPointRatio>0.6 && projPointRatio<=1.0)
			{
				return 0.3;
			}
		}

}

float Graph::ComputeDistance(Point &p1,Point &p2)
{
	float dx=p1.x-p2.x;
	float dy=p1.y-p2.y;
	return sqrt(dx*dx+dy*dy);
}

void Graph::ComputeBezierInterPoint()
{
	for (int i=0;i<mBezierCurveList.size();i++)
	{
		Point ctrlPt[3];
		if (mBezierCurveList[i].kind == START)
		{
			//�����һ���ߵĵ�һ�����������ߣ�����ʼ��Ϊ�ߵ���ʼ��
			ctrlPt[0] = mEdgePoints[abs(mBezierCurveList[i].start)];
			ctrlPt[1] = ComuputeInterPointInRoad(abs(mBezierCurveList[i].end),mBezierCurveList[i].startProjectPtRatio);
			ctrlPt[2] = ComuputeInterPointInRoad(abs(mBezierCurveList[i].end),mBezierCurveList[i].endRoadRatio);
		}
		else if (mBezierCurveList[i].kind == END)
		{
			//�����һ���ߵ����һ�����������ߣ�����ֹ��Ϊ�ߵ���ֹ��
			ctrlPt[0] = ComuputeInterPointInRoad(abs(mBezierCurveList[i].start),mBezierCurveList[i].startRoadRatio);
			ctrlPt[1] = ComuputeInterPointInRoad(abs(mBezierCurveList[i].start),mBezierCurveList[i].endProjectPtRatio);
			ctrlPt[2] = mEdgePoints[abs(mBezierCurveList[i].end)];
		}
		else
		{
			ctrlPt[0] = ComuputeInterPointInRoad(abs(mBezierCurveList[i].start),mBezierCurveList[i].startRoadRatio);
			ctrlPt[1] = mRoadPoint[mBezierCurveList[i].middleCtrlPt];
			ctrlPt[2] = ComuputeInterPointInRoad(abs(mBezierCurveList[i].end),mBezierCurveList[i].endRoadRatio);
		}
		CreateOneBezier(mBezierCurveList[i].interPoint,ctrlPt,30);
	}
}

Point Graph::ComuputeInterPointInRoad( int roadIndex,float ratio )
{
	Point newPoint;
	Point startPt = mRoadPoint[mRoadList[roadIndex].startPoint];
	Point endPt = mRoadPoint[mRoadList[roadIndex].endPoint];
	newPoint.x = startPt.x+(endPt.x-startPt.x)*ratio;
	newPoint.y = startPt.y+(endPt.y-startPt.y)*ratio;
	return newPoint;
}
osg::Geode* Graph::CreateRoadGeom()
{
	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArray = new osg::Vec4Array;
	for (int i=1;i<mRoadNum;i++)
	{
		pointArray->push_back(osg::Vec4(mRoadPoint[mRoadList[i].startPoint].x,mRoadPoint[mRoadList[i].startPoint].y,0.1,1.0));
		pointArray->push_back(osg::Vec4(mRoadPoint[mRoadList[i].endPoint].x,mRoadPoint[mRoadList[i].endPoint].y,0.1,1.0));
	}
	osg::Vec4Array* colorArray = new osg::Vec4Array;
	colorArray->push_back(osg::Vec4(0.0,0.0,1.0,1.0));

	osg::Geometry* roadGeom = new osg::Geometry;
	roadGeom->setColorArray(colorArray);
	roadGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	roadGeom->setVertexArray(pointArray);
	roadGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,(mRoadNum-1)*2));
	//roadGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,(mRoadList.size()-1)*2));


	osg::LineWidth* linewidth = new osg::LineWidth();

	linewidth->setWidth(2.0);
	roadGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
	graphGeode->addDrawable(roadGeom);
	return graphGeode;
}
osg::Geode* Graph::CreateEdgeGeom()
{
	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArray = new osg::Vec4Array;
	for (int i=0;i<mUnPassEdgeNUM.size();i++)
	{
		int edgenum = mUnPassEdgeNUM[i];
		pointArray->push_back(osg::Vec4(mEdgePoints[mEdgeList[edgenum].startPoint].x,mEdgePoints[mEdgeList[edgenum].startPoint].y,0.0,1.0));
		pointArray->push_back(osg::Vec4(mEdgePoints[mEdgeList[edgenum].endPoint].x,mEdgePoints[mEdgeList[edgenum].endPoint].y,0.0,1.0));
	}
	    osg::Vec4Array* colorArray = new osg::Vec4Array;
		colorArray->push_back(osg::Vec4(0.2,0.2,0.2,1.0));
		osg::Geometry* edgeGeom = new osg::Geometry;
		edgeGeom->setColorArray(colorArray);
		edgeGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
		edgeGeom->setVertexArray(pointArray);
		edgeGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,(mUnPassEdgeNUM.size())*2));
		osg::LineWidth* linewidth = new osg::LineWidth();
		linewidth->setWidth(1.0);
		edgeGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
		graphGeode->addDrawable(edgeGeom);

	
	
	return graphGeode;

}
osg::Geode* Graph::CreateALLEdgeGeom()
{
	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArray = new osg::Vec4Array;
	for (int i=0;i<mEdgeList.size();i++)
	{

		pointArray->push_back(osg::Vec4(mEdgePoints[mEdgeList[i].startPoint].x,mEdgePoints[mEdgeList[i].startPoint].y,0.0,1.0));
		pointArray->push_back(osg::Vec4(mEdgePoints[mEdgeList[i].endPoint].x,mEdgePoints[mEdgeList[i].endPoint].y,0.0,1.0));
	}
	osg::Vec4Array* colorArray = new osg::Vec4Array;
	colorArray->push_back(osg::Vec4(0.2,0.2,0.2,1.0));
	osg::Geometry* edgeGeom = new osg::Geometry;
	edgeGeom->setColorArray(colorArray);
	edgeGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	edgeGeom->setVertexArray(pointArray);
	edgeGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,(mEdgeList.size()*2)));

	osg::LineWidth* linewidth = new osg::LineWidth();
	linewidth->setWidth(1.0);
	edgeGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
	graphGeode->addDrawable(edgeGeom);



	return graphGeode;

    


}

osg::Geode* Graph::CreateAllEdgePoint()
{
	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArrayStart = new osg::Vec4Array;
	osg::Vec4Array* pointArrayEnd = new osg::Vec4Array;
	for (int i=0;i<mEdgeList.size();i++)
	{
		
		pointArrayStart->push_back(osg::Vec4(mEdgePoints[mEdgeList[i].startPoint].x,mEdgePoints[mEdgeList[i].startPoint].y,0.0,1.0));
		pointArrayEnd->push_back(osg::Vec4(mEdgePoints[mEdgeList[i].endPoint].x,mEdgePoints[mEdgeList[i].endPoint].y,0.0,1.0));
	}
	
	osg::Vec4Array* colorArrayStart = new osg::Vec4Array;
	colorArrayStart->push_back(osg::Vec4(1.0,0.0,0.0,1.0));

	osg::Vec4Array* colorArrayEnd = new osg::Vec4Array;
	colorArrayEnd->push_back(osg::Vec4(0.0,1.0,1.0,1.0));

	osg::Geometry* edgeGeom = new osg::Geometry;
	edgeGeom->setColorArray(colorArrayStart);
	edgeGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	edgeGeom->setVertexArray(pointArrayStart);
	edgeGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,mEdgeList.size()));
	//edgeGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,(mEdgeList.size())*2));

	osg::Geometry* edgeGeom2 = new osg::Geometry;
	edgeGeom2->setColorArray(colorArrayEnd);
	edgeGeom2->setColorBinding(osg::Geometry::BIND_OVERALL);
	edgeGeom2->setVertexArray(pointArrayEnd);
	edgeGeom2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,mEdgeList.size()));


    
	osg::Point* point = new osg::Point;
	point->setSize(3.0);
	edgeGeom->getOrCreateStateSet()->setAttributeAndModes(point,osg::StateAttribute::ON);
	edgeGeom2->getOrCreateStateSet()->setAttributeAndModes(point,osg::StateAttribute::ON);

	/*osg::LineWidth* linewidth = new osg::LineWidth();
	linewidth->setWidth(1.0);
	edgeGeom->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);*/
	graphGeode->addDrawable(edgeGeom);
	graphGeode->addDrawable(edgeGeom2);



	return graphGeode;

}

osg::Geode* Graph::CreateRedBezierGeom()
{

	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArray = new osg::Vec4Array;
	int lastIndex=0;
	for (int j=0;j<mRedBezierCurveList.size();j++)
	{
		osg::Geometry* curve = new osg::Geometry;		
		for (int i=0;i<mBezierCurveList[mRedBezierCurveList[j].BezierCurveNum].interPoint.size();i++)
		{
			pointArray->push_back(osg::Vec4(mBezierCurveList[mRedBezierCurveList[j].BezierCurveNum].interPoint[i].x,mBezierCurveList[mRedBezierCurveList[j].BezierCurveNum].interPoint[i].y,0.05,1.0));
		}

		osg::Vec4Array* colorArray = new osg::Vec4Array;
		colorArray->push_back(osg::Vec4(1.0,0.0,0.0,1.0));

		curve->setColorArray(colorArray);
		curve->setColorBinding(osg::Geometry::BIND_OVERALL);
		curve->setVertexArray(pointArray);
		curve->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,lastIndex,mBezierCurveList[mRedBezierCurveList[j].BezierCurveNum].interPoint.size()));

		lastIndex += mBezierCurveList[mRedBezierCurveList[j].BezierCurveNum].interPoint.size();
		osg::LineWidth* linewidth = new osg::LineWidth();

		linewidth->setWidth(mRedBezierCurveList[j].value*0.1+2);
		curve->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
		graphGeode->addDrawable(curve);
	}
	return graphGeode;
}

osg::Geode* Graph::CreateColorBezierGeom()
{
	osg::Geode* graphGeode = new osg::Geode;
	osg::Vec4Array* pointArray = new osg::Vec4Array;
    osg::Vec4Array* colorArray = new osg::Vec4Array;
	int lastIndex=0;
	int colorBezierSize = mColorBezierCurveList.size();
	osg::Image *colorImage = osgDB::readImageFile("color.png");
	for (int j=0;j<colorBezierSize;j++)
	{
		osg::Geometry* curve = new osg::Geometry;
		int interPointSize = mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].interPoint.size();
		for (int i=0;i<interPointSize;i++)
		{
			pointArray->push_back(osg::Vec4(mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].interPoint[i].x,mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].interPoint[i].y,0.2,1.0));
		}
		float color = (mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].endtex - mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].statrtex)/(float)(mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].interPoint.size()-1);
		for (int i=0;i<interPointSize;i++)
		{
			
			float colori = float(i)*color + mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].statrtex ;
			osg::Vec4 colorputer = colorImage->getColor(osg::Vec3(colori,0.5,0.5));
			colorputer.a() = 1.0;

			colorArray->push_back(colorputer);
		}
		
		curve->setColorArray(colorArray);		
		curve->setVertexArray(pointArray);
		curve->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		curve->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,lastIndex,mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].interPoint.size()));

		lastIndex += mBezierCurveList[mColorBezierCurveList[j].BezierCurveNum].interPoint.size();
		osg::LineWidth* linewidth = new osg::LineWidth();

		linewidth->setWidth(mColorBezierCurveList[j].value*0.1+2);
		curve->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
		graphGeode->addDrawable(curve);
	}
	return graphGeode;

}

bool Graph::SetEdgePointsVisible()
{
	

}

bool Graph::SetRoadListVisible(int i)
{
	mRoadCurve->setNodeMask(i);
	
	if (i==0)//�������ʾ
	{
		return true;
	} 
	else
	{
		return false;
	}


}

bool Graph::SetBezierCurveListVisible( int i )
{
	mBezierCurve->setNodeMask(i);
	mDrawEdgePoints->setNodeMask(i);
	if (i==0)//�������ʾ
	{
		return true;//�´��ٰ�������ʾ
	} 
	else
	{
		return false;
	}
	

}

bool Graph::SetColorBezierCurveListVisible( int i )
{
	mColorBezierCurve->setNodeMask(i);
	if (i==0)//�������ʾ
	{
		return true;
	} 
	else
	{
		return false;
	}

}
