// FreeEndDectet.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FreeEndDetect.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "memory.h"
/*端点和节点检测*/
struct Point
{
	int x;
	int y;
};
/*领域点判断表，其中0为中间点，1为端点，2为节点*/
static int EndAndJuntion[256]=
{
	 0,1,1,1,1,0,1,0,       1,0,0,0,1,0,0,0,//1:0-15
	 1,0,0,0,0,2,0,0,       1,0,0,0,0,0,0,0,//2:16-31
	 1,0,0,0,0,2,0,0,       0,2,0,0,0,2,0,0,//3:32-47
	 1,0,0,0,0,2,0,0,       0,2,0,0,0,0,0,0,//4:48-63
	 1,0,0,0,0,2,0,0,       0,2,2,2,0,2,2,0,//5:64-79
	 0,2,2,2,2,2,2,0,       0,2,2,0,0,0,0,0,//6:80-95
	 1,0,0,0,0,2,0,0,       0,2,0,0,0,0,0,0,//7:96-111
	 0,0,0,0,0,0,0,0,       0,0,0,0,0,0,0,0,//8:112-127
	 1,1,0,0,0,0,0,0,       0,0,0,0,0,0,0,0,//9:128-143
	 0,0,2,2,2,2,2,0,       0,0,0,0,0,0,0,0,//10:144-159
	 0,0,0,0,2,2,0,0,       0,0,2,0,0,0,0,0,//11:160-175
	 0,0,0,0,2,0,0,0,       0,0,0,0,0,0,0,0,//12:176-191
	 1,0,0,0,0,0,0,0,       0,0,0,0,0,0,0,0,//13:192-207
	 0,0,2,0,2,0,0,0,       0,0,0,0,0,0,0,0,//14:208-223
	 0,0,0,0,2,0,0,0,       0,0,0,0,0,0,0,0,//15:224-239
	 0,0,0,0,0,0,0,0,       0,0,0,0,0,0,0,0,//16:240-255
};
static float EightNeibor[8]=
{
	1/255,	2/255,	4/255,
	128/255,		8/255,
	64/255,	32/255, 16/255
};
//static int FreeEndValue[8]=
//{
//	1,2,4,8,16,32,64,128,//pole shape
//	3,6,12，24,48,96,192,129//L shape
//};
//static int Junctions[22]=
//{
//	85,//四联通
//	149,53,101,77,83,89,86,212,//四联通点
//	210,165,75,150,45,90,180,105,170,//四联通点2
//	84,81,69,21,//三个角点，90度旋转
//	82,164,73,146,37,74,148,41//七字型，45度旋转
//};
// 计算领域特征值
int EightNeiborCalulate(unsigned char *pi,unsigned int x,unsigned int y,unsigned int width)
{
	int Value = pi[(y-1)*width+(x-1)]/255 + pi[(y-1)*width+(x)]/255*2 + pi[(y-1)*width+(x+1)]/255*4
					+pi[y*width+(x-1)]/255*128 + pi[y*width+x+1]/255*8	
					+pi[(y+1)*width+(x-1)]/255*64 + pi[(y+1)*width+(x)]/255*32 + pi[(y+1)*width+(x+1)]/255*16;
	return Value;
}
/*判别顶点的特征值*/
bool FreeEndCalculate(int value)
{
	if( EndAndJuntion[value]==1)
		return true;
	else
		return false;
}
/*判别结点的特征值*/
bool JunctionsCalculate(int value)
{
	if( EndAndJuntion[value]==2)
		return true;
	else
		return false;
}
/*对图像边缘进行扩展*/
bool Expand(unsigned char *Array,unsigned char *pi,unsigned int _width, unsigned int _height)
{
	unsigned int x,y;
	for(y=0; y<_height;y++)//注意防止越界
	{
		for(x=0;x<_width;x++)
		{
			if(x==0||x==_width-1||y==0||y==_height-1)
			{
				pi[y * _width + x] = 0;
			}
			else
			{
				pi[y * _width + x] = Array[(y-1) * (_width-2) + x-1];
			}
		}
	}
	return true;
}
/*对图像边缘进行恢复*/
bool Erode(unsigned char *Array,unsigned char *pi,unsigned int _width, unsigned int _height)
{
	unsigned int x,y;
	for(y=1; y<_height-1;y++)//注意防止越界
	{
		for(x=1;x<_width-1;x++)
		{
			
			Array[(y-1) * (_width-2) + x-1] = pi[y * _width + x];
		}
	}
	return true;
}
int FreeEndDetect(unsigned char *Array,unsigned int BWWidth,unsigned int BWHeight, int *EndArray, int *JunctionArray)
{
	unsigned int x,y;
	unsigned char *pi;
	struct Point *FreeEnd = new struct Point[1000];
	struct Point *Junctions = new struct Point[1000];
	unsigned int M=0;
	unsigned int N=0;
	unsigned int _Width = BWWidth +2;
	unsigned int _Height = BWHeight +2;
	int BufSize = _Width*_Height;
	int Value = 0;
	pi = new unsigned char[BufSize];
	if(!Expand(Array,pi,_Width,_Height)) 
		return 1;
	for(y=1;y<_Height-1;y++)
		for(x=1;x<_Width-1;x++)
		{
			if(pi[y * _Width + x] == 255)
			{
				Value = EightNeiborCalulate(pi,x, y,_Width);
				if( FreeEndCalculate(Value))
				{
					FreeEnd[N].x = x-1;
					FreeEnd[N].y = y-1;
					Array[(y-1) * (_Width-2) + x-1] = 1;
					N++;
				}
				else if(JunctionsCalculate(Value))
				{
					Junctions[M].x = x-1;
					Junctions[M].y = y-1;
					Array[(y-1) * (_Width-2) + x-1] = 2;
					M++;
				}
			}
		}
	//EndArray = new unsigned int[N*2];
	for(y=0;y<N;y++)
	{
		EndArray[y*2] = FreeEnd[y].x;
		EndArray[y*2+1] = FreeEnd[y].y;
	}
	//JunctionArray = new unsigned int[M*2];
	for(y=0;y<M;y++)
	{
		JunctionArray[y*2] = Junctions[y].x;
		JunctionArray[y*2+1] =Junctions[y].y;
	}
	delete pi;
	delete FreeEnd;
	delete Junctions;
	return EndArray[10];
}


