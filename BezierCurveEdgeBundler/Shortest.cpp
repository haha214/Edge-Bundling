#include "Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <Windows.h>
XYline Graph::MakeLine(int startnum,int endnum) //两点确定一条线段
{

	XYline line;

	float x1 = mRoadPoint[startnum].x;

	float y1 = mRoadPoint[startnum].y;

	float x2 = mRoadPoint[endnum].x;

	float y2 = mRoadPoint[endnum].y;

	line.A=(y2-y1);

	line.B=(x1-x2);

	line.C=y1*(x2-x1)+x1*(y1-y2);

	line.RoadStart = startnum;

	line.RoadEnd = endnum;

	return line;

}

float Graph::dis_PL(Point P,XYline line) //点到直线距离
{
	float x = P.x;

	float y = P.y;

	return fabs(line.A*x + line.B*y + line.C) / sqrt(line.A*line.A + line.B*line.B);

}
float Graph::dis_PP(Point p1,Point p2)//两点间距离
{
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;

   return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

}

Point Graph::ProjectPointToLine(Point s,Point e,Point p)//点到直线垂点的位置
{
	float x1 = s.x;
	float y1 = s.y;
	float x2 = e.x;
	float y2 = e.y;
	float x = p.x;
	float y = p.y;
	Point np;
	float L = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
	float r = ((y1-y)*(y1-y2) - (x1-x)*(x2-x1)) / (L * L);

	np.x=x1 + r * (x2-x1);
	np.y=y1 + r * (y2-y1);
	return np;
}
//点到线段的距离,如果点的垂点落在了线段的延长线上，则用点到离其最近的线段端点的距离代替点到线段的距离
float Graph::dis_PS(Point p,XYline line)
{
	int startnum = line.RoadStart;
	int endnum = line.RoadEnd;
	float dis;
	int Index;
	Point Vp = ProjectPointToLine(mRoadPoint[startnum],mRoadPoint[endnum],p);//垂点
	float Vp_s = dis_PP(Vp,mRoadPoint[startnum]);
	float Vp_e = dis_PP(Vp,mRoadPoint[endnum]);
	float s_e = dis_PP(mRoadPoint[startnum],mRoadPoint[endnum]);
	Index = (Vp_s>s_e || Vp_e>s_e)?0:1;//垂点是否落在线段的延长线上，是为0，不是为1
	if (Index==0)
	{
		int num = (Vp_s<Vp_e)?startnum:endnum;
		
		dis = dis_PP(p,mRoadPoint[num]);
	} 
	else
	{
		dis = dis_PL(p,line);
	}
	return dis;
}

float Graph::Per(Point p,XYline line)//计算边两端端点在路上的垂点在路上所占的比例
{
	int startnum = line.RoadStart;
	int endnum = line.RoadEnd;
	float per;
	int Index;
	Point Vp = ProjectPointToLine(mRoadPoint[startnum],mRoadPoint[endnum],p);//垂点
	float Vp_s = dis_PP(Vp,mRoadPoint[startnum]);
	float Vp_e = dis_PP(Vp,mRoadPoint[endnum]);
	float s_e = dis_PP(mRoadPoint[startnum],mRoadPoint[endnum]);
	Index = (Vp_s>s_e || Vp_e>s_e)?0:1;//垂点是否落在线段的延长线上，是为0，不是为1
	if (Index==0)
	{
		per = (Vp_s<Vp_e)?0.0:1.0;

	} 
	else
	{
		per = Vp_s/s_e;
	}
	return per;

}

//如果点P的垂点落在了线段的延长线上，index为0，则s1num是离点P较近的点的数组下标，s2num为较远的；如果垂点落在线段内，则s1num为线段的起点
void Graph::Judgment(Point p,XYline line,int& Index,int& s1num,int& s2num)
{
	int startnum = line.RoadStart;
	int endnum = line.RoadEnd;
	Point Vp = ProjectPointToLine(mRoadPoint[startnum],mRoadPoint[endnum],p);//垂点
	float Vp_s = dis_PP(Vp,mRoadPoint[startnum]);
	float Vp_e = dis_PP(Vp,mRoadPoint[endnum]);
	float s_e = dis_PP(mRoadPoint[startnum],mRoadPoint[endnum]);
	Index = (Vp_s>=s_e || Vp_e>=s_e)?0:1;//垂点是否落在线段的延长线上，是为0，不是为1
	if (Index==0)
	{
		s1num = (Vp_s<Vp_e)?startnum:endnum;
		s2num = (Vp_s>Vp_e)?startnum:endnum;
		
	} 
	else
	{
		s1num = startnum;
		s2num = endnum;
		
	}
	
}

//点到哪条线段距离最短，linenum是这条线段在XYline中的数组下标，s1num是这条线段的起始端点，s2num是线段的终止端点，index等于0时，点P的垂点在线段的延长线上，最短路径中不包含P的垂点
void Graph::Sdis_PL(Point P,std::vector<XYline> line,int& Index,int& s1num,int& s2num,int& linenum)
{
	//XYline closet;
	
	float cldis = dis_PS(P,line[1]);
	/*int num = 0;*/
	int linesize = line.size();
	for (int i=2;i<linesize;i++)//0号边不参加运算，默认离下标为1的路最近，因此从2号路还是比较
	{
		float dis = dis_PS(P,line[i]);
		if (dis<cldis)
		{
			cldis = dis;
			linenum = i;
		}
	}
	Judgment(P,line[linenum],Index,s1num,s2num);
}
void Graph::Dijkstra(int n,std::vector<std::vector<RoadValue>> map,int s1,int s2,int e1,int e2,int index1,int index2,Edge& edge,int edgenum)//路上一共n个点，map[u][v]表示点u到v的边的权重，若uv之间无直接相连的边，则权重为无穷大，源点s，终点e，源点s离路的s1s2边最近，终点e离路的e1e2最近，容器shortest中存放从点e到点s之间的最短路径，容器RoadPoint中存放路中的所有点的信息，edgenum是当前参与计算边的序号
{
	int esnum = edge.startPoint;
	int eenum = edge.endPoint;
	Point s = mEdgePoints[esnum];
	Point e = mEdgePoints[eenum];

	



	bool *p = new bool[n];//记录该点是否属于Va，不属于Va的点属于Vb
	int *pre = new int[n];//记录最短路径终点的前驱结点

	int *prenum = new int[n];//记录最短路径走到当前结点所通过的路的序号

	std::vector<int> fshortest;//保存的是最短路径的倒序
	//std::vector<Point> shortest;//保存最终的最短路径
	float *dist = new float[n];//dist[i]表示第i个点到路上源点s的最短距离
	Point VerticalPointS;//起始线段上的垂点
	Point VerticalPointE;//终止线段上的垂点
	float L1 = -1.0;//终止线段上垂点到e1的距离，初始化设为-1
	float L2 = -1.0;//终止线段上垂点到e2的距离，初始化设为-1
	//初始化
	if (index1==0)//无垂点，源点是s,初始化时路的源点是s1
	{
		for(int i=0;i<=n-1;i++)//初始化
		{
			p[i]=false;
			if (i!=s1)
			{
				RoadValue v = map[s1][i];
				dist[i]= v.roadvalue;
				pre[i]=s1;
				prenum[i]=v.roadnum;
			}

		}
		dist[s1]=0.0;
		pre[s1] = -1;
		prenum[s1] = 0;
		p[s1]=true;

	} 
	else//index1==1,有垂点，源点是s，初始化时路的源点是起始垂点VerticalPointS
	{
		VerticalPointS = ProjectPointToLine(mRoadPoint[s1],mRoadPoint[s2],s);
		for(int i=0;i<=n-1;i++)//初始化路上各个结点到起始垂点的距离
		{
			p[i] = false;
			dist[i] = 1e+6;
			pre[i]= -1;
		}
		dist[s1]=dis_PP(VerticalPointS,mRoadPoint[s1]);
		dist[s2]=dis_PP(VerticalPointS,mRoadPoint[s2]);
		RoadValue v = map[s1][s2];
		prenum[s2] = v.roadnum;//垂点只与s1和s2相连，因此只需初始化s1和s2的prenum
		prenum[s1] = -(v.roadnum);
	}

	for (int i =0;i<=n-1;i++)//循环n次，求s到其他n-1个点的最短路径或垂点vps到其他路上所有的n个点的最短路径，当计算到s与e的最短路径后，跳出循环
	{
		float min = 1e+5;
		int k = -2;
		for (int j=0;j<=n-1;j++)//在Vb中的点取一s到其距离最小的点k
		{
			if (!p[j]&&dist[j]<min)
			{
				min = dist[j];
				k=j;
			}

		}
		if (k == -2) //没有可扩展的点，即剩下的点均不可达，说明此时路与路之间不全部具有连通性，离当前边的两个端点最近的两条路无法到达
		{
			printf("两点之间无连通性\n");
			edge.roadPassed.push_back(0);//直接将边绘制出来
			return;
		}


		p[k] = true;

		if (index2==0 && k==e1)//路上的终点是e1,e在路上的垂点在其延长线上，e1是离e距离较近的那个结点
		{
	
			int a = k;
			while(a!=-1)
			{
				fshortest.push_back(prenum[a]);
				a = pre[a];
			}
			if (index1==0)//路上的起始点是s1
			{
				fshortest.pop_back();//删除最后一个元素，即起始点s1前面不存在路上的边
			}

			int size= fshortest.size();
			for (int i=0;i<size;i++)
			{
				edge.roadPassed.push_back(fshortest[size-1-i]);
				if ((fshortest[size-1-i])>0)//将当前边的序号保存到其所通过的路中，如果正向通过则保存为正数，反向通过为负数
				{
					//mRoadList[fshortest[size-1-i]].edgePassed.push_back(edgenum);
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(edgenum);//将当前边的序号保存到其所通过的路中
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(-edgenum);//将当前边的序号保存到其所通过的路中
					ReleaseMutex(hMutex);
					
				}

				
			}
			return;
		}
		if ((k==e1 || k==e2)&& index2==1)//路上的终点是vpe
		{
			VerticalPointE = ProjectPointToLine(mRoadPoint[e1],mRoadPoint[e2],e);
			float el = dis_PP(VerticalPointE,mRoadPoint[k]);
			int a = pre[k];
			int b = k;
			float LL = el;
			while(a!=-1)
			{
				LL = LL+map[a][b].roadvalue;
				b = pre[b];
				a = pre[a];
			}
			if (index1==1)//路上的起点是vps
			{
				LL = LL+dist[b];
			}
			if (k==e1)
			{
				L1 = LL;
			} 
			else
			{
				L2 = LL;
			}
			if (L1>0 && L2>0)
			{
				int PenPoint = (L1<=L2)?e1:e2;//路上的倒数第二个点,倒数第一个点是VPE
				int lnum = map[e1][e2].roadnum;
				int LastRoad = (PenPoint==e1)?lnum:(-lnum);
				fshortest.push_back(LastRoad);//垂点与倒数第二个点之间的路
				while(PenPoint!=-1)
				{
					fshortest.push_back(prenum[PenPoint]);
					PenPoint = pre[PenPoint];
				}

				if (index1==0)//路上的起始点是s1
				{
					fshortest.pop_back();//删除最后一个元素，即起始点s1前面不存在路上的边
				}
				int size = fshortest.size();
				for (int i=0;i<size;i++)
				{
					edge.roadPassed.push_back(fshortest[size-1-i]);

					if ((fshortest[size-1-i])>0)//将当前边的序号保存到其所通过的路中
					{
						extern HANDLE hMutex;
						
						WaitForSingleObject(hMutex,INFINITE);
						//("从线程进入互斥区！");
						mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(edgenum);//将当前边的序号保存到其所通过的路中
						ReleaseMutex(hMutex);
						

					} 
					else
					{
						extern HANDLE hMutex;
					
						WaitForSingleObject(hMutex,INFINITE);
						//("从线程进入互斥区！");
						mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(-edgenum);//将当前边的序号保存到其所通过的路中
						ReleaseMutex(hMutex);
						
					}

				}
				return;
			}

		}


		for(int j=0;j<=n-1;j++)
		{
			RoadValue v = map[k][j];
			//对于每个与k相连的在Vb中的点j，更新s到j的最短路径
			if (!p[j] && (v.roadvalue<(1e+5)) && (dist[j]>dist[k]+v.roadvalue))
			{
				dist[j] = dist[k]+v.roadvalue;
				pre[j] = k;
				prenum[j] = v.roadnum;
			}

		}
	}
	delete p;
	delete pre;
	delete prenum;
	delete dist;

}

Point Graph::MakeVector (Point p1,Point p2)
{
	Point p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}

float Graph::Cos(Point v1,Point v2)
{
	float dot = v1.x*v2.x + v1.y*v2.y;
	float v1norm = sqrt(v1.x*v1.x + v1.y*v1.y);
	float v2norm = sqrt(v2.x*v2.x + v2.y*v2.y);
	return dot / (v1norm * v2norm);

}

void Graph::AngleMeasure(Edge e,float factor)//最短路径中相邻路径进行角度判断，如果两条相邻路径夹角很大，就合并路径生成一条新的路径
{
	if (e.roadPassed[0]!=0)//边的两端通过所给路径可以到达
	{
		//int elsize = mEdgeList[i].roadPassed.size();
		for (int j = 1;j<(e.roadPassed.size() - 2);)//第一条路和最后一条路因为有边两端的投影，因此不参与合并
		{
			Point p1;//当前路径的起始点
			Point p2;//当前路径的终止点，对于接下来走的相邻路径则为其起点
			Point p3;//当前路径的下一条路径的终止点
			int newrdsn;//新路起始结点
			int newrden;//新路的终止结点
			/*int p2num1,p2num2;*/
			if (e.roadPassed[j]<0)//判断路径方向
			{
				newrdsn = mRoadList[abs(e.roadPassed[j])].endPoint;
				p1 = mRoadPoint[newrdsn];
				p2 = mRoadPoint[mRoadList[abs(e.roadPassed[j])].startPoint];
				//p2num1 = mRoadList[abs(e.roadPassed[j])].startPoint;


			} 
			else
			{
				newrdsn = mRoadList[e.roadPassed[j]].startPoint;
				p1 = mRoadPoint[newrdsn];
				p2 = mRoadPoint[mRoadList[e.roadPassed[j]].endPoint];
				//p2num1 = mRoadList[e.roadPassed[j]].endPoint;

			}
			if (e.roadPassed[j+1]<0)
			{
				newrden = mRoadList[abs(e.roadPassed[j+1])].startPoint;
				p3 = mRoadPoint[newrden];
				//p2num2 = mRoadList[abs(e.roadPassed[j+1])].endPoint;
			} 
			else
			{
				newrden = mRoadList[e.roadPassed[j+1]].endPoint;
				p3 = mRoadPoint[newrden];
				//p2num2 = mRoadList[e.roadPassed[j+1]].startPoint;
			}
			Point v1 = MakeVector(p1,p2);//当前路径构建的向量
			Point v2 = MakeVector(p3,p2);//下一条路径构建的向量
			float cosvalue = Cos(v1,v2);//两向量夹角的COS值
			if (cosvalue< factor)//合并两条路
			{
				Road rd;//构建的新的路径
				rd.startPoint = newrdsn;
				rd.endPoint = newrden;
				rd.value = 1.0;
				mRoadList.push_back(rd);
				int newrdnum = mRoadList.size() - 1;//新路的数组下标
				e.roadPassed[j] = newrdnum;//替换当前路径
				std::vector<int>::iterator it;
				it = e.roadPassed.begin();
				e.roadPassed.erase(it+j+1);//下一条路径已经和当前路径合并成新路径，因此删除下一条路径

			} 
			else//不合并
			{
				j++;
			}
		}

	}


}

//void Graph::FERoad(Edge e,Road r,int num)//num是边e的数组下标
//{
//	int s = e.roadPassed[0];//边e最短路径通过的第一条路
//	int size  = e.roadPassed.size();
//	int e = e.roadPassed[size-1];//边e最短路径通过的最后一条路
//	if (s<0)//反向通过路s
//	{
//		mRoadList[abs(s)].firstnum.push_back(-num);//将边的序号的负值赋给路s
//	} 
//	else
//	{
//		mRoadList[s].firstnum.push_back(num);
//	}
//	if (e<0)//反向通过路e
//	{
//		mRoadList[abs(e)].endnum.push_back(-num);
//	} 
//	else
//	{
//		mRoadList[e].endnum.push_back(num);
//	}
//
//}

void Graph::ComputeShortestPath(int EdgeStart,int EdgeEnd)
{
	
	std::vector<XYline> l;
	int RoadPointNum = mRoadPoint.size();//路的结点个数
	int RoadNum = mRoadList.size();//路的边的条数
	//int EdgeNum = mEdgeList.size();//图上的边的条数
	//int EdgeNum = EdgeEnd - EdgeStart + 1;//当前线程需要计算边的条数
	RoadValue ev;
	ev.roadvalue = 1e+6;
	ev.roadnum = 1e+6;
	std::vector<RoadValue> v(RoadPointNum,ev);
	std::vector<std::vector<RoadValue>> map(RoadPointNum,v);
	for (int i=1;i<RoadNum;i++)
	{
		Road r = mRoadList[i];
		int roadstartnum = r.startPoint;
		int roadendnum = r.endPoint;
		float roadlength = dis_PP(mRoadPoint[roadstartnum],mRoadPoint[roadendnum]);
		RoadValue es;
		RoadValue ee;
		es.roadvalue = r.value * roadlength;
		ee.roadvalue = r.value * roadlength;
		es.roadnum = i;
		ee.roadnum = -i;

		map[roadendnum][roadstartnum] = ee;
		map[roadstartnum][roadendnum] = es;

	}
	XYline L0;
	l.push_back(L0);
	
	for (int i=1;i<RoadNum;i++)
	{
		Road r = mRoadList[i];
		l.push_back(MakeLine(r.startPoint,r.endPoint));

	}

	for (int i=EdgeStart;i<=EdgeEnd;i++)
	{
		int index1,index2;
		int s1num,s2num;
		int e1num,e2num;
		int linenumstart = 1;
		int linenumend = 1;
		int edgestartnum = mEdgeList[i].startPoint;
		int edgeendnum = mEdgeList[i].endPoint;
		Sdis_PL(mEdgePoints[edgestartnum],l,index1,s1num,s2num,linenumstart);

		Sdis_PL(mEdgePoints[edgeendnum],l,index2,e1num,e2num,linenumend);

		mEdgeList[i].startProjPoint = Per(mEdgePoints[edgestartnum],l[linenumstart]);
		mEdgeList[i].endProjPoint = Per(mEdgePoints[edgeendnum],l[linenumend]);

		if (s1num==e1num && s2num==e2num)//边两端的端点投影在同一条路上
		{

			if (index1==0 && index2==0)
			{
				
				mEdgeList[i].roadPassed.push_back(0);//不经过路
				mUnPassEdgeNUM.push_back(i);
				
			}
			if (index1==0 && index2==1)
			{
				
				RoadValue e = map[s1num][s2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				
				if (e.roadnum>0)                                 //i号边通过了roadnum号路
				{
					//mRoadList[e.roadnum].edgePassed.push_back(i);
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					//mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					
				}

				
			}
			if (index1==1 && index2==0)
			{
				
		
				RoadValue e = map[s2num][s1num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i号边通过了roadnum号路
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					

				} 
				else
				{
                    extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
				
				}
				
			}
			if (index1==1 && index2==1)
			{
				

				Point VPS = ProjectPointToLine(mRoadPoint[s1num],mRoadPoint[s2num],mEdgePoints[edgestartnum]);
				Point VPE = ProjectPointToLine(mRoadPoint[e1num],mRoadPoint[e2num],mEdgePoints[edgeendnum]);
				float x = dis_PP(mRoadPoint[s1num],VPS);
				float y = dis_PP(mRoadPoint[s1num],VPE);
				if (x==y)//边与路垂直
				{
					mEdgeList[i].roadPassed.push_back(0);//不经过路
					mUnPassEdgeNUM.push_back(i);
				} 
				else
				{
					RoadValue e;
					e = (x<y)?map[s1num][s2num]:map[s2num][s1num];//通过比较垂点与路的起点s1的距离来判断路的方向
					mEdgeList[i].roadPassed.push_back(e.roadnum);

					if (e.roadnum>0)                                 //i号边通过了roadnum号路
					{
						extern HANDLE hMutex;
						WaitForSingleObject(hMutex,INFINITE);
						//("从线程进入互斥区！");
						mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i号边通过了roadnum号路
						ReleaseMutex(hMutex);

					} 
					else
					{
						extern HANDLE hMutex;
						WaitForSingleObject(hMutex,INFINITE);
						//("从线程进入互斥区！");
						mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i号边通过了roadnum号路
						ReleaseMutex(hMutex);
					}
				}
				
				
			}
		}
		else if (s1num==e2num && s2num==e1num)//边两端的端点投影在同一条路上
		{
			
			if (index1==0 && index2==0)
			{

				RoadValue e = map[s1num][s2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i号边通过了roadnum号路
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					
				}
			}
			if (index1==1 && index2==0)
			{

				RoadValue e = map[s1num][s2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i号边通过了roadnum号路
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					
				}
			}
			if (index1==0 && index2==1)
			{

				RoadValue e = map[e1num][e2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i号边通过了roadnum号路
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("从线程进入互斥区！");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i号边通过了roadnum号路
					ReleaseMutex(hMutex);
					
				}
			}
		
		}
		else
		{
			Dijkstra(RoadPointNum,map,s1num,s2num,e1num,e2num,index1,index2,mEdgeList[i],i);
			//AngleMeasure(mEdgeList[i],-0.95);


			   
		}
		
	}

}

