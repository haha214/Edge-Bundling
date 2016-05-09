#include "Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <Windows.h>
XYline Graph::MakeLine(int startnum,int endnum) //����ȷ��һ���߶�
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

float Graph::dis_PL(Point P,XYline line) //�㵽ֱ�߾���
{
	float x = P.x;

	float y = P.y;

	return fabs(line.A*x + line.B*y + line.C) / sqrt(line.A*line.A + line.B*line.B);

}
float Graph::dis_PP(Point p1,Point p2)//��������
{
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;

   return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

}

Point Graph::ProjectPointToLine(Point s,Point e,Point p)//�㵽ֱ�ߴ����λ��
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
//�㵽�߶εľ���,�����Ĵ����������߶ε��ӳ����ϣ����õ㵽����������߶ζ˵�ľ������㵽�߶εľ���
float Graph::dis_PS(Point p,XYline line)
{
	int startnum = line.RoadStart;
	int endnum = line.RoadEnd;
	float dis;
	int Index;
	Point Vp = ProjectPointToLine(mRoadPoint[startnum],mRoadPoint[endnum],p);//����
	float Vp_s = dis_PP(Vp,mRoadPoint[startnum]);
	float Vp_e = dis_PP(Vp,mRoadPoint[endnum]);
	float s_e = dis_PP(mRoadPoint[startnum],mRoadPoint[endnum]);
	Index = (Vp_s>s_e || Vp_e>s_e)?0:1;//�����Ƿ������߶ε��ӳ����ϣ���Ϊ0������Ϊ1
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

float Graph::Per(Point p,XYline line)//��������˶˵���·�ϵĴ�����·����ռ�ı���
{
	int startnum = line.RoadStart;
	int endnum = line.RoadEnd;
	float per;
	int Index;
	Point Vp = ProjectPointToLine(mRoadPoint[startnum],mRoadPoint[endnum],p);//����
	float Vp_s = dis_PP(Vp,mRoadPoint[startnum]);
	float Vp_e = dis_PP(Vp,mRoadPoint[endnum]);
	float s_e = dis_PP(mRoadPoint[startnum],mRoadPoint[endnum]);
	Index = (Vp_s>s_e || Vp_e>s_e)?0:1;//�����Ƿ������߶ε��ӳ����ϣ���Ϊ0������Ϊ1
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

//�����P�Ĵ����������߶ε��ӳ����ϣ�indexΪ0����s1num�����P�Ͻ��ĵ�������±꣬s2numΪ��Զ�ģ�������������߶��ڣ���s1numΪ�߶ε����
void Graph::Judgment(Point p,XYline line,int& Index,int& s1num,int& s2num)
{
	int startnum = line.RoadStart;
	int endnum = line.RoadEnd;
	Point Vp = ProjectPointToLine(mRoadPoint[startnum],mRoadPoint[endnum],p);//����
	float Vp_s = dis_PP(Vp,mRoadPoint[startnum]);
	float Vp_e = dis_PP(Vp,mRoadPoint[endnum]);
	float s_e = dis_PP(mRoadPoint[startnum],mRoadPoint[endnum]);
	Index = (Vp_s>=s_e || Vp_e>=s_e)?0:1;//�����Ƿ������߶ε��ӳ����ϣ���Ϊ0������Ϊ1
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

//�㵽�����߶ξ�����̣�linenum�������߶���XYline�е������±꣬s1num�������߶ε���ʼ�˵㣬s2num���߶ε���ֹ�˵㣬index����0ʱ����P�Ĵ������߶ε��ӳ����ϣ����·���в�����P�Ĵ���
void Graph::Sdis_PL(Point P,std::vector<XYline> line,int& Index,int& s1num,int& s2num,int& linenum)
{
	//XYline closet;
	
	float cldis = dis_PS(P,line[1]);
	/*int num = 0;*/
	int linesize = line.size();
	for (int i=2;i<linesize;i++)//0�ű߲��μ����㣬Ĭ�����±�Ϊ1��·�������˴�2��·���ǱȽ�
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
void Graph::Dijkstra(int n,std::vector<std::vector<RoadValue>> map,int s1,int s2,int e1,int e2,int index1,int index2,Edge& edge,int edgenum)//·��һ��n���㣬map[u][v]��ʾ��u��v�ıߵ�Ȩ�أ���uv֮����ֱ�������ıߣ���Ȩ��Ϊ�����Դ��s���յ�e��Դ��s��·��s1s2��������յ�e��·��e1e2���������shortest�д�Ŵӵ�e����s֮������·��������RoadPoint�д��·�е����е����Ϣ��edgenum�ǵ�ǰ�������ߵ����
{
	int esnum = edge.startPoint;
	int eenum = edge.endPoint;
	Point s = mEdgePoints[esnum];
	Point e = mEdgePoints[eenum];

	



	bool *p = new bool[n];//��¼�õ��Ƿ�����Va��������Va�ĵ�����Vb
	int *pre = new int[n];//��¼���·���յ��ǰ�����

	int *prenum = new int[n];//��¼���·���ߵ���ǰ�����ͨ����·�����

	std::vector<int> fshortest;//����������·���ĵ���
	//std::vector<Point> shortest;//�������յ����·��
	float *dist = new float[n];//dist[i]��ʾ��i���㵽·��Դ��s����̾���
	Point VerticalPointS;//��ʼ�߶��ϵĴ���
	Point VerticalPointE;//��ֹ�߶��ϵĴ���
	float L1 = -1.0;//��ֹ�߶��ϴ��㵽e1�ľ��룬��ʼ����Ϊ-1
	float L2 = -1.0;//��ֹ�߶��ϴ��㵽e2�ľ��룬��ʼ����Ϊ-1
	//��ʼ��
	if (index1==0)//�޴��㣬Դ����s,��ʼ��ʱ·��Դ����s1
	{
		for(int i=0;i<=n-1;i++)//��ʼ��
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
	else//index1==1,�д��㣬Դ����s����ʼ��ʱ·��Դ������ʼ����VerticalPointS
	{
		VerticalPointS = ProjectPointToLine(mRoadPoint[s1],mRoadPoint[s2],s);
		for(int i=0;i<=n-1;i++)//��ʼ��·�ϸ�����㵽��ʼ����ľ���
		{
			p[i] = false;
			dist[i] = 1e+6;
			pre[i]= -1;
		}
		dist[s1]=dis_PP(VerticalPointS,mRoadPoint[s1]);
		dist[s2]=dis_PP(VerticalPointS,mRoadPoint[s2]);
		RoadValue v = map[s1][s2];
		prenum[s2] = v.roadnum;//����ֻ��s1��s2���������ֻ���ʼ��s1��s2��prenum
		prenum[s1] = -(v.roadnum);
	}

	for (int i =0;i<=n-1;i++)//ѭ��n�Σ���s������n-1��������·���򴹵�vps������·�����е�n��������·���������㵽s��e�����·��������ѭ��
	{
		float min = 1e+5;
		int k = -2;
		for (int j=0;j<=n-1;j++)//��Vb�еĵ�ȡһs���������С�ĵ�k
		{
			if (!p[j]&&dist[j]<min)
			{
				min = dist[j];
				k=j;
			}

		}
		if (k == -2) //û�п���չ�ĵ㣬��ʣ�µĵ�����ɴ˵����ʱ·��·֮�䲻ȫ��������ͨ�ԣ��뵱ǰ�ߵ������˵����������·�޷�����
		{
			printf("����֮������ͨ��\n");
			edge.roadPassed.push_back(0);//ֱ�ӽ��߻��Ƴ���
			return;
		}


		p[k] = true;

		if (index2==0 && k==e1)//·�ϵ��յ���e1,e��·�ϵĴ��������ӳ����ϣ�e1����e����Ͻ����Ǹ����
		{
	
			int a = k;
			while(a!=-1)
			{
				fshortest.push_back(prenum[a]);
				a = pre[a];
			}
			if (index1==0)//·�ϵ���ʼ����s1
			{
				fshortest.pop_back();//ɾ�����һ��Ԫ�أ�����ʼ��s1ǰ�治����·�ϵı�
			}

			int size= fshortest.size();
			for (int i=0;i<size;i++)
			{
				edge.roadPassed.push_back(fshortest[size-1-i]);
				if ((fshortest[size-1-i])>0)//����ǰ�ߵ���ű��浽����ͨ����·�У��������ͨ���򱣴�Ϊ����������ͨ��Ϊ����
				{
					//mRoadList[fshortest[size-1-i]].edgePassed.push_back(edgenum);
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(edgenum);//����ǰ�ߵ���ű��浽����ͨ����·��
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(-edgenum);//����ǰ�ߵ���ű��浽����ͨ����·��
					ReleaseMutex(hMutex);
					
				}

				
			}
			return;
		}
		if ((k==e1 || k==e2)&& index2==1)//·�ϵ��յ���vpe
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
			if (index1==1)//·�ϵ������vps
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
				int PenPoint = (L1<=L2)?e1:e2;//·�ϵĵ����ڶ�����,������һ������VPE
				int lnum = map[e1][e2].roadnum;
				int LastRoad = (PenPoint==e1)?lnum:(-lnum);
				fshortest.push_back(LastRoad);//�����뵹���ڶ�����֮���·
				while(PenPoint!=-1)
				{
					fshortest.push_back(prenum[PenPoint]);
					PenPoint = pre[PenPoint];
				}

				if (index1==0)//·�ϵ���ʼ����s1
				{
					fshortest.pop_back();//ɾ�����һ��Ԫ�أ�����ʼ��s1ǰ�治����·�ϵı�
				}
				int size = fshortest.size();
				for (int i=0;i<size;i++)
				{
					edge.roadPassed.push_back(fshortest[size-1-i]);

					if ((fshortest[size-1-i])>0)//����ǰ�ߵ���ű��浽����ͨ����·��
					{
						extern HANDLE hMutex;
						
						WaitForSingleObject(hMutex,INFINITE);
						//("���߳̽��뻥������");
						mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(edgenum);//����ǰ�ߵ���ű��浽����ͨ����·��
						ReleaseMutex(hMutex);
						

					} 
					else
					{
						extern HANDLE hMutex;
					
						WaitForSingleObject(hMutex,INFINITE);
						//("���߳̽��뻥������");
						mRoadList[abs(fshortest[size-1-i])].edgePassed.push_back(-edgenum);//����ǰ�ߵ���ű��浽����ͨ����·��
						ReleaseMutex(hMutex);
						
					}

				}
				return;
			}

		}


		for(int j=0;j<=n-1;j++)
		{
			RoadValue v = map[k][j];
			//����ÿ����k��������Vb�еĵ�j������s��j�����·��
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

void Graph::AngleMeasure(Edge e,float factor)//���·��������·�����нǶ��жϣ������������·���нǺܴ󣬾ͺϲ�·������һ���µ�·��
{
	if (e.roadPassed[0]!=0)//�ߵ�����ͨ������·�����Ե���
	{
		//int elsize = mEdgeList[i].roadPassed.size();
		for (int j = 1;j<(e.roadPassed.size() - 2);)//��һ��·�����һ��·��Ϊ�б����˵�ͶӰ����˲�����ϲ�
		{
			Point p1;//��ǰ·������ʼ��
			Point p2;//��ǰ·������ֹ�㣬���ڽ������ߵ�����·����Ϊ�����
			Point p3;//��ǰ·������һ��·������ֹ��
			int newrdsn;//��·��ʼ���
			int newrden;//��·����ֹ���
			/*int p2num1,p2num2;*/
			if (e.roadPassed[j]<0)//�ж�·������
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
			Point v1 = MakeVector(p1,p2);//��ǰ·������������
			Point v2 = MakeVector(p3,p2);//��һ��·������������
			float cosvalue = Cos(v1,v2);//�������нǵ�COSֵ
			if (cosvalue< factor)//�ϲ�����·
			{
				Road rd;//�������µ�·��
				rd.startPoint = newrdsn;
				rd.endPoint = newrden;
				rd.value = 1.0;
				mRoadList.push_back(rd);
				int newrdnum = mRoadList.size() - 1;//��·�������±�
				e.roadPassed[j] = newrdnum;//�滻��ǰ·��
				std::vector<int>::iterator it;
				it = e.roadPassed.begin();
				e.roadPassed.erase(it+j+1);//��һ��·���Ѿ��͵�ǰ·���ϲ�����·�������ɾ����һ��·��

			} 
			else//���ϲ�
			{
				j++;
			}
		}

	}


}

//void Graph::FERoad(Edge e,Road r,int num)//num�Ǳ�e�������±�
//{
//	int s = e.roadPassed[0];//��e���·��ͨ���ĵ�һ��·
//	int size  = e.roadPassed.size();
//	int e = e.roadPassed[size-1];//��e���·��ͨ�������һ��·
//	if (s<0)//����ͨ��·s
//	{
//		mRoadList[abs(s)].firstnum.push_back(-num);//���ߵ���ŵĸ�ֵ����·s
//	} 
//	else
//	{
//		mRoadList[s].firstnum.push_back(num);
//	}
//	if (e<0)//����ͨ��·e
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
	int RoadPointNum = mRoadPoint.size();//·�Ľ�����
	int RoadNum = mRoadList.size();//·�ıߵ�����
	//int EdgeNum = mEdgeList.size();//ͼ�ϵıߵ�����
	//int EdgeNum = EdgeEnd - EdgeStart + 1;//��ǰ�߳���Ҫ����ߵ�����
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

		if (s1num==e1num && s2num==e2num)//�����˵Ķ˵�ͶӰ��ͬһ��·��
		{

			if (index1==0 && index2==0)
			{
				
				mEdgeList[i].roadPassed.push_back(0);//������·
				mUnPassEdgeNUM.push_back(i);
				
			}
			if (index1==0 && index2==1)
			{
				
				RoadValue e = map[s1num][s2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				
				if (e.roadnum>0)                                 //i�ű�ͨ����roadnum��·
				{
					//mRoadList[e.roadnum].edgePassed.push_back(i);
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					//mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					
				}

				
			}
			if (index1==1 && index2==0)
			{
				
		
				RoadValue e = map[s2num][s1num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i�ű�ͨ����roadnum��·
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					

				} 
				else
				{
                    extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
				
				}
				
			}
			if (index1==1 && index2==1)
			{
				

				Point VPS = ProjectPointToLine(mRoadPoint[s1num],mRoadPoint[s2num],mEdgePoints[edgestartnum]);
				Point VPE = ProjectPointToLine(mRoadPoint[e1num],mRoadPoint[e2num],mEdgePoints[edgeendnum]);
				float x = dis_PP(mRoadPoint[s1num],VPS);
				float y = dis_PP(mRoadPoint[s1num],VPE);
				if (x==y)//����·��ֱ
				{
					mEdgeList[i].roadPassed.push_back(0);//������·
					mUnPassEdgeNUM.push_back(i);
				} 
				else
				{
					RoadValue e;
					e = (x<y)?map[s1num][s2num]:map[s2num][s1num];//ͨ���Ƚϴ�����·�����s1�ľ������ж�·�ķ���
					mEdgeList[i].roadPassed.push_back(e.roadnum);

					if (e.roadnum>0)                                 //i�ű�ͨ����roadnum��·
					{
						extern HANDLE hMutex;
						WaitForSingleObject(hMutex,INFINITE);
						//("���߳̽��뻥������");
						mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i�ű�ͨ����roadnum��·
						ReleaseMutex(hMutex);

					} 
					else
					{
						extern HANDLE hMutex;
						WaitForSingleObject(hMutex,INFINITE);
						//("���߳̽��뻥������");
						mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i�ű�ͨ����roadnum��·
						ReleaseMutex(hMutex);
					}
				}
				
				
			}
		}
		else if (s1num==e2num && s2num==e1num)//�����˵Ķ˵�ͶӰ��ͬһ��·��
		{
			
			if (index1==0 && index2==0)
			{

				RoadValue e = map[s1num][s2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i�ű�ͨ����roadnum��·
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					
				}
			}
			if (index1==1 && index2==0)
			{

				RoadValue e = map[s1num][s2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i�ű�ͨ����roadnum��·
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					
				}
			}
			if (index1==0 && index2==1)
			{

				RoadValue e = map[e1num][e2num];
				mEdgeList[i].roadPassed.push_back(e.roadnum);

				if (e.roadnum>0)                                 //i�ű�ͨ����roadnum��·
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(i);//i�ű�ͨ����roadnum��·
					ReleaseMutex(hMutex);
					

				} 
				else
				{
					extern HANDLE hMutex;
					
					WaitForSingleObject(hMutex,INFINITE);
					//("���߳̽��뻥������");
					mRoadList[abs(e.roadnum)].edgePassed.push_back(-i);//i�ű�ͨ����roadnum��·
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

