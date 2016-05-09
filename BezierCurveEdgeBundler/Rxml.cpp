#include<iostream>
#include<fstream>
#include <vector>
#include <string>
#include <math.h>
//#include "tinystr.h"
//#include "tinyxml.h"
#include "Graph.h"

bool Graph::initPointsFromXML(TiXmlElement* graphEle,int n)
{
	if (!graphEle)
	{
		return false;
	}
	TiXmlElement* pNodeEle;
	int i;
	for (pNodeEle=graphEle->FirstChildElement("node"),i=0;pNodeEle && i<n;pNodeEle=pNodeEle->NextSiblingElement("node"))
	{
		Point p;
		TiXmlElement* pDataEle;
		for (pDataEle=pNodeEle->FirstChildElement("data");pDataEle;pDataEle=pDataEle->NextSiblingElement("data"))
		{
			std::string key = pDataEle->Attribute("key");

			std::string num = pDataEle->GetText();
			if (key == "x")
			{
				p.x = atof(num.c_str());
				p.x = p.x/10.0;
			}
			else if (key == "y")
			{
				p.y = atof(num.c_str());
				p.y = fabs(p.y/10.0);
			}			
		}
		mEdgePoints.push_back(p);	
		i++;
	}
	return true;

}

bool Graph::initEdgeFromXML(TiXmlElement* graphEle,int n,int EdgePointNum)
{
	if (!graphEle)
	{
		return false;
	}
	/*float evMin = pow(10.0,10.0), evMax = -pow(10.0,10.0);*/
	TiXmlElement* pEdgeEle;
	int i;
	for (pEdgeEle=graphEle->FirstChildElement("edge"),i=0;pEdgeEle && i<n;pEdgeEle=pEdgeEle->NextSiblingElement("edge"))
	{
		std::string sourceID = pEdgeEle->Attribute("source");
		std::string targetID = pEdgeEle->Attribute("target");
		int sID,tID;
		sID = atoi(sourceID.c_str());
		tID = atoi(targetID.c_str());
		if (sID<EdgePointNum&&tID<EdgePointNum)
		{
			Edge e;
			e.startPoint = sID;
			e.endPoint = tID;
			mEdgeList.push_back(e);
			/*edgeStarts.push_back(endPoints[sID]);
			edgeEnds.push_back(endPoints[tID]);*/
		/*	length = computeDistance(endPoints[sID],endPoints[tID]);
			if (abs(length) < EPS) length = 0.0;
			edgeLengths.push_back(length);*/

			//TiXmlElement* pDataEle=pEdgeEle->FirstChildElement("data");
			//string valueS = pDataEle->GetText();
			//float _value = atof(valueS.c_str());
			//edgeValues.push_back(_value);
			//if(_value>evMax)
			//{
			//	evMax=_value;
			//}
			//if(_value<evMin)
			//{
			//	evMin=_value;
			//}
			//numEdges++;
		}
		i++;
	}
	return true;
}

bool Graph::initGraphInfo(int EdgePointNum,int EdgeNum)
{
	TiXmlDocument* pDoc = new TiXmlDocument;
	if (NULL == pDoc)
	{
		return false;
	}
	if (!pDoc->LoadFile("../data/bundling_data/migrations.xml"))
	{
		std::cout<<"打开文件失败！"<< std::endl;
		return false;
	}
	TiXmlElement* pRootEle = pDoc->RootElement();
	if (pRootEle)
	{
		TiXmlElement* pGraphEle = pRootEle->FirstChildElement("graph");
		if (pGraphEle&&initPointsFromXML(pGraphEle,EdgePointNum)&&initEdgeFromXML(pGraphEle,EdgeNum,EdgePointNum)/*&&initRoadGrid()*/)
		{		
			delete pDoc;
			return true;
		}
		else
		{
			delete pDoc;
			return false;
		}
	}
	else
	{
		delete pDoc;
		return false;
	}
}

//void main()
//{
//	int EdgePointNUM = 100;
//	int EdgeNUM = 50;
//	std::vector<Point> EdgePoint;
//	std::vector<Edge> EdgeList;
//	bool b = initGraphInfo(EdgePointNUM,EdgePoint,EdgeNUM,EdgeList);
//	int EdgePointSize = EdgePoint.size();
//	int EdgeSize = EdgeList.size();
//	std::cout<<EdgePointSize<<" "<<EdgeSize<<std::endl;
//	for (int i= 0;i<EdgePointSize;i++)
//	{
//		std::cout<<EdgePoint[i].x<<" "<<EdgePoint[i].y<<std::endl;
//
//	}
//	for (int i = 0;i<EdgeSize;i++)
//	{
//		std::cout<<EdgeList[i].startPoint<<" "<<EdgeList[i].endPoint<<std::endl;
//	}
//	return;
//
//
//}