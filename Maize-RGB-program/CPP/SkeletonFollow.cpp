// SkeletonFollow.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SkeletonFollow.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "memory.h"
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
			if(Array[(y-1) * (_width-2) + x-1]!=1&&Array[(y-1) * (_width-2) + x-1]!=2)			
				Array[(y-1) * (_width-2) + x-1] = pi[y * _width + x];
		}
	}
	return true;
}
int Calulate8Neibor(unsigned char *Array, int x, int y, unsigned int BWWidth)
{
	int i,j;
	int N=0;
	for(j=-1;j<2;j++)
	{
		for(i=-1;i<2;i++)
		{
			if(i == 0 && j ==0)
				continue;
			else if(Array[(y+j)*BWWidth + x + i])
				N++;
		}
	}
	return N;
}
int Calulate4Neibor(unsigned char *Array, int &x, int &y, unsigned int BWWidth)
{
	int i,j;
	for(j=-1;j<2;j++)
	{
		for(i=-1;i<2;i++)
		{
			if(Array[(y+j)*BWWidth + x + i]&&abs(i+j)==1)
			{
				if(Array[(y+j)*BWWidth + x + i] == 2)
				{
					return 0;
				}
				else
				{
					x = x+i;
					y = y+j;
					return 1;
				}
			}
		}
	}
	return 0;
}
int NextNeibor(unsigned char *Array, int &x, int &y, unsigned int BWWidth)
{
	int i,j;
	for(j=-1;j<2;j++)
	{
		for(i=-1;i<2;i++)
		{
			if(Array[(y+j)*BWWidth + x + i])
			{
				if(Array[(y+j)*BWWidth + x + i] == 2)
				{
					return 0;
				}
				else if(Array[(y+j)*BWWidth + x + i])
				{
					x = x+i;
					y = y+j;
					return 1;
				}
			}
		}
	}
	return 0;
}
//int EndToJunction(unsigned char *Array,unsigned char *pArray,int x,int y, int BWWidth,int label)
int EndToJunction(unsigned char *Array,unsigned char *pArray,int x,int y, int BWWidth,int label)
{
	while(1)
	{
		int value = Calulate8Neibor(Array,x,y,BWWidth);
		if(value==1)
		{
			Array[y*BWWidth+x] = 0;
			pArray[y*BWWidth+x] = label;
			if(NextNeibor(Array,x,y,BWWidth) == 0)
			{
				return 1;
			}
		}
		else if(value>1)
		{
			Array[y*BWWidth+x] = 0;
			pArray[y*BWWidth+x] = label;
			if(Calulate4Neibor(Array,x,y,BWWidth) == 0)
			{
				return 1;
			}
		}
		else
		{
			Array[y*BWWidth+x] = 0;//叶片标记2015.4.2加
			pArray[y*BWWidth+x] = label;//叶片标记2015.4.2加
			break;
		}
	}
	return 1;
}
int SkeletonFollow(unsigned char *p,unsigned int _BWWidth,unsigned int _BWHeight)
//int SkeletonFollow(unsigned char *p,unsigned int _BWWidth,unsigned int _BWHeight, unsigned int _start=0, unsigned int _length= 1500,unsigned int _dead=0)
{
	unsigned int x,y;
	int Label =3;
	unsigned int BWWidth = _BWWidth +2;
	unsigned int BWHeight = _BWHeight +2;
	unsigned char *Array, *pArray;
	int BufSize = BWWidth*BWHeight;
	pArray = new unsigned char[BufSize];
	Array  = new unsigned char[BufSize];
	if(!Expand(p,Array,BWWidth,BWHeight)) 
		return 1;
	if(!Expand(p,pArray,BWWidth,BWHeight)) 
		return 1;
	for(y=1;y<BWHeight-1;y++)
	//for(y=BWHeight-1;y>0;y--)
	{
		for(x=1;x<BWWidth-1;x++)
		//for(x=_start;x<_start + _length;x++)
		{
			if(Array[y*BWWidth+x] == 1)
			{
				//EndToJunction(Array,pArray,x,y,BWWidth,Label+_dead);
				EndToJunction(Array,pArray,x,y,BWWidth,Label);
				Label++;
			}
		}
	}
	for(y=1;y<BWHeight-1;y++)
	//for(y=BWHeight-1;y>0;y--)
	{
		//for(x=_start;x<_start + _length;x++)
		for(x=1;x<BWWidth-1;x++)
		{
			if(Array[y*BWWidth+x] == 2)
			{
				Array[y*BWWidth+x]=0;
				int i;
				int j;
				int k = 0;
				int m = 0;
				int n = 0;
				unsigned char *value = new unsigned char[4];
				for(j= -1;j<2;j++)
				{
					for(i=-1;i<2;i++)
					{
						if(abs(i+j)==1)
						{
							value[k] = Array[(y+j)*BWWidth+x+i];
							Array[(y+j)*BWWidth+x+i] = 0;
							k++;
						}
					}
				}
				for(j= -1;j<2;j++)
				{
					for(i=-1;i<2;i++)
					{
						if(abs(i+j)==1)
						{
							if(value[m]==255)
							{
								Array[(y+j)*BWWidth+x+i] = value[m];
								value[m]				 = 0;	
								EndToJunction(Array,pArray,x+i,y+j,BWWidth,Label);
								Label++;
							}
							m++;
						}
					}
				}
				for(j= -1;j<2;j++)
				{
					for(i=-1;i<2;i++)
					{
						if(abs(i+j)==1)
						{
							if(value[n]==2)
							{
								Array[(y+j)*BWWidth+x+i] = value[n];
								value[n]				 = 0;
							}
							n++;
						}
					}
				}
				for(j= -1;j<2;j++)
				{
					for(i=-1;i<2;i++)
					{
						if(Array[(y+j)*BWWidth+x+i]==255&&abs(i)+abs(j)==2)
						{
							EndToJunction(Array,pArray,x+i,y+j,BWWidth,Label);
							Label++;
						}
					}
				}
				delete value;
			}
		}

	}

		
	if(!Erode(p,pArray,BWWidth,BWHeight))
		return 1;
	delete pArray;
	delete Array;
	return 1;
}
