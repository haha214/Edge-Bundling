#include <Windows.h>
#include "Graph.h"
#include <process.h>
#include <iostream>
#include <time.h>


void Graph::ComputeThread(void* para)
{
	MultiThread* mt = (MultiThread*)para;
	Graph* object = mt->ptr;
	int edgestartnum = mt->startnum;
	int edgeendnum = mt->endnum;
	object->ComputeShortestPath(edgestartnum,edgeendnum);
	_endthread();


}

void Graph::ThreadComputeSP()
{
	int EdgeNum = mEdgeList.size();
	int PartNum;
	int SurNum;

	time_t start,end;
	start = clock();

    

	if (EdgeNum%mThreadSize == 0)
	{
		PartNum = EdgeNum/mThreadSize;
		MultiThread* mt = new MultiThread[mThreadSize];
		for (int i=0;i<mThreadSize;i++)
		{
			mt[i].ptr = this;
			mt[i].startnum = i*PartNum;
			mt[i].endnum = (i+1)*PartNum - 1;
		}
		HANDLE* handles = new HANDLE[mThreadSize];
		for (int j = 0;j<mThreadSize;j++)
		{
			handles[j] = (HANDLE)_beginthread(&Graph::ComputeThread,0,(void*)(mt+j));
		}
		WaitForMultipleObjects(mThreadSize,handles,true,INFINITE);
		delete []mt;
		delete []handles;

	} 
	else
	{
		PartNum = EdgeNum/mThreadSize;
		SurNum = EdgeNum % mThreadSize;
		MultiThread* mt = new MultiThread[mThreadSize+1];
		for(int i=0;i<mThreadSize;i++)
		{
			mt[i].ptr = this;
			mt[i].startnum = i*PartNum;
			mt[i].endnum = (i+1)*PartNum - 1; 
		}
		mt[mThreadSize].ptr = this;
		mt[mThreadSize].startnum = PartNum * mThreadSize;
		mt[mThreadSize].endnum = EdgeNum - 1;
		HANDLE* handles = new HANDLE[mThreadSize+1];
		for (int j = 0;j<=mThreadSize;j++)
		{
			handles[j] = (HANDLE)_beginthread(&Graph::ComputeThread,0,(void*)(mt+j));
		}
		WaitForMultipleObjects(mThreadSize+1,handles,true,INFINITE);
		delete []mt;
		delete []handles;

	}
	end = clock();
	std::cout<<"16线程同时运行的时间为"<<(double)(end-start)<<std::endl;


}


void Graph::SetThreadNum( int n )
{
	mThreadSize = n;

}
