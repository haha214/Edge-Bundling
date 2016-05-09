#include<iostream>
#include<fstream>
#include<stdio.h>
#include<math.h>
#include <vector>
#include "Graph.h"
//using namespace std;
//char ss[8];
//char ee[8];



//λ��ת������
//long ChangeByteOrder(long data)
//{
//     for(int i = 0; i < 8; i ++)
//     {
//          ss[i] = '0';
//          ee[i] = '0';
//     }
//     unsigned long val = (unsigned long)(data);
//     _ultoa(val, ss, 16);
//     int length = strlen(ss); 
//     
//     for(int i = 0; i < 8 - length; i ++)
//     ee[i] = '0';
//     for(int i = 0; i < length; i ++)
//     ee[i + 8 - length] = ss[i];
//
//     //���е���
//     long t;
//     t = ee[0];
//     ee[0] = ee[6];
//     ee[6] = t;
//     t = ee[1];
//     ee[1] = ee[7];
//     ee[7] = t;
//     t = ee[2];
//     ee[2] = ee[4];
//     ee[4] = t;
//     t = ee[3];
//     ee[3] = ee[5];
//     ee[5] = t;
//
//     //������ʮ��������val���ַ���ss�е�ʮ������ת�����ǽ�����
//     int value = 0;
//     for(int i = 0; i < 8; i ++)
//     {
//         int k;
//         if(ee[i] == 'a' || ee[i] == 'b' || ee[i] == 'c' || ee[i] == 'd' || ee[i] == 'e' || ee[i] == 'f')
//         k = 10 + ee[i] - 'a';
//         else
//         k = ee[i] - '0';
//         value = value * 16 + k;
//     } 
//     return value;
//}

 
//��ȡ�����ļ�(.shp)���ļ�ͷ
void Graph::ReadShp(std::string ShpFileName,std::vector<PolyLine>& PL)
{
     FILE *m_ShpFile;
     if((m_ShpFile = fopen(ShpFileName.c_str(), "rb")) == NULL)
     {
		 std::cout << "��ȡshp�ļ�ʧ��!" << std::endl;
          return;
     }
     //��ȡ�����ļ�ͷ������
     int FileCode;
     int Unused;
     int FileLength;
     int Version = 0;
     int ShapeType = 100;
     double Xmin = 0;
     double Ymin = 0;
     double Xmax = 0;
     double Ymax = 0;
     double Zmin;
     double Zmax;
     double Mmin;
     double Mmax;
     fread(&FileCode, sizeof(int), 1, m_ShpFile);
	 //std::cout << ChangeByteOrder(FileCode) << std::endl;
     for(int i = 0; i < 5; i ++)
     {
         fread(&Unused, sizeof(int), 1, m_ShpFile);
		 //std::cout << ChangeByteOrder(Unused) << std::endl;
     }
     fread(&FileLength,   sizeof(int),   1,m_ShpFile);
	 //std::cout << ChangeByteOrder(FileLength) << std::endl;
     
     fread(&Version,      sizeof(int),   1,m_ShpFile);
	 //std::cout << Version << std::endl;
     
     fread(&ShapeType,    sizeof(int),   1,m_ShpFile);
	 //std::cout << ShapeType << std::endl;
     
     fread(&Xmin,         sizeof(double),1,m_ShpFile);
     fread(&Ymin,         sizeof(double),1,m_ShpFile);
     fread(&Xmax,         sizeof(double),1,m_ShpFile);
     fread(&Ymax,         sizeof(double),1,m_ShpFile);
     
	 /*std::cout << Xmin << " " << Xmax << std::endl;
	 std::cout << Ymin << " " << Ymax << std::endl;*/
     
     fread(&Zmin,         sizeof(double),1,m_ShpFile);
     fread(&Zmax,         sizeof(double),1,m_ShpFile);
     fread(&Mmin,         sizeof(double),1,m_ShpFile);
     fread(&Mmax,         sizeof(double),1,m_ShpFile);
     
	 //std::cout << Zmin << " " << Zmax << std::endl;
	 //std::cout << Mmin << " " << Mmax << std::endl;
     //��ȡ�����ļ�ͷ�����ݽ���
     

     //��ȡ��״Ŀ���ʵ����Ϣ
     int RecordNumber;
     int ContentLength;
     int num = 0;

     while(fread(&RecordNumber, sizeof(int), 1, m_ShpFile) != 0)
     {
         fread(&ContentLength, sizeof(int), 1, m_ShpFile);
         /*RecordNumber = ChangeByteOrder(RecordNumber);
         ContentLength = ChangeByteOrder(ContentLength);*/
         int shapeType;
         double box[4];
         int NumberParts;
         int NumberPoints;
         int *Parts;
         //�������� shapeType
         fread(&shapeType, sizeof(int), 1, m_ShpFile);
         //���귶Χbox 
         for(int i = 0; i < 4; i ++)
		 {
            fread(box + i, sizeof(double), 1, m_ShpFile);
		

		 }
         //���߶θ���NumParts 
         fread(&NumberParts, sizeof(int), 1, m_ShpFile);

         //�������NumPoints 
         fread(&NumberPoints, sizeof(int),1, m_ShpFile);
		 //cout<<NumberPoints<<endl;
         //Parts���� 
         Parts = new int[NumberParts];
         for(int i = 0; i < NumberParts;i ++)
         fread(Parts + i, sizeof(int), 1, m_ShpFile);
         //Points���� 
         int pointNum;
         for(int i = 0; i < NumberParts;i ++)
         {
             if(i != NumberParts - 1)
             pointNum = Parts[i + 1] - Parts[i];
             else
             pointNum = NumberPoints - Parts[i];
			 std::vector<Point> P(pointNum);
             double *PointsX;
             double *PointsY;
             PointsX = new double[pointNum];
             PointsY = new double[pointNum];
             for(int j = 0; j < pointNum; j ++)
             {
				 fread(PointsX + j, sizeof(double), 1, m_ShpFile);
                 fread(PointsY + j, sizeof(double), 1, m_ShpFile);
				//cout << PointsX[j] << " " << PointsY[j] << endl;
				 				 
             }
			 for(int j = 0;j<pointNum;j++)
			 {
				 P[j].x = float(PointsX[j]);
				 P[j].y = float(PointsY[j]);
			 }
			 for (int i = 0;i<(pointNum-1);i++)
			 {
				 float sl = fabs(P[i].x - P[i+1].x);
				 float el = fabs(P[i].y - P[i+1].y);
				 if (sl>0.001 || el>0.001)
				 { 
					 PolyLine L;
				     L.s = P[i];
				     L.e = P[i+1];
					 PL.push_back(L);
					 
				 }
				
			 }

             delete[] PointsX;
             delete[] PointsY;
         }
         delete[] Parts;
     } 

}

int Graph::SamePoint(Point p1,Point p2)
{
	int index;
	float d1 = fabs(p1.x - p2.x);
	float d2 = fabs(p1.y - p2.y);
	if (d1<0.001 && d2<0.001)
	{
		index = 0;//p1��p2��ͬһ���㣬����0
	} 
	else
	{
		index = 1;
	}
	return index;

}

void Graph::MakeRoad(std::vector<PolyLine> PL)
{
	int size = PL.size();
	Point s = PL[0].s;
	Point e = PL[0].e;
	mRoadPoint.push_back(s);
	mRoadPoint.push_back(e);
	Road r0 ;
	r0.startPoint = 0;
	r0.endPoint = 1;
	r0.value = 1.0;
	mRoadList.push_back(r0);
	int rpindex = 1;
	for (int i = 1;i<size;i++)
	{
		Point p1 = PL[i].s;
		Point p2 = PL[i].e;
		//int rpsize = RoadPoint.size();
		int index1;
		int index2;
		Road rd;
		for(int j = 0;j<=rpindex;j++)
		{
			index1 = SamePoint(p1,mRoadPoint[j]);
			if (index1 == 0)//��ͬһ����
			{
				rd.startPoint = j;
				break;
			}
		}
		if (index1 == 1)
		{
			mRoadPoint.push_back(p1);
			rpindex = rpindex+1;
			rd.startPoint = rpindex;
		}
		for (int j = 0;j<=rpindex;j++)
		{
			index2 = SamePoint(p2,mRoadPoint[j]);
			if (index2 == 0)
			{
				rd.endPoint = j;
				break;
			}
		}
		if (index2 == 1)
		{
			mRoadPoint.push_back(p2);
			rpindex = rpindex+1;
			rd.endPoint = rpindex;
		}
		rd.value = 1.0;
		mRoadList.push_back(rd);
	}
}
void Graph::RshpMroad(std::string ShpFileName)
{
	std::vector<PolyLine> PL;
	ReadShp(ShpFileName,PL);
	MakeRoad(PL);
}

//int main()
//{
//	std::vector<PolyLine> pl;
//	std::vector<Point> P;
//	std::vector<Road> R;
//
//    ReadShp("E:\\data1\\in15oc03\\in101503.shp",pl);
//	MakeRoad(pl,P,R);
//	//ReadShp("E:\\data1\\USA_rds\\USA_roads.shp");
//    /*system("pause");*/
//    return 0;
//}
