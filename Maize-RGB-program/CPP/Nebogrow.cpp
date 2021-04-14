// Nebogrow.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Nebogrow.h"
#include "math.h"
bool Compute(unsigned int pValue)
{
	unsigned int value;
	unsigned char r,g,b;
	r=g=b=0;

	value = pValue/256;
	b = pValue%256;
	g = value%256;
	r = value/256;

	if(g > 240||2*g -b - r > 130)
	{
		return true;
	}
	else return false;
}

int Nebogrow (unsigned int *pImag, //原始彩色图像
				 unsigned char *Graybuff, //预分割灰度图
				 unsigned int BWWidth, //图像宽
				 unsigned int BWHeight) //图像高度
{
	//图像一维矩阵索引,及r,g,b值
	unsigned int i,j;
	unsigned char r,g,b;
	r=g=b=0;

	unsigned int value;
	unsigned int Exg=0;
	unsigned int index=0;

	//从上往下扫描，重新判断
	for(j=0;j<BWHeight-1;j++)
	{
		for(i=1;i<BWWidth-1;i++)
		{
			if(Graybuff[j*BWWidth + i]==1)
			{
				if(Graybuff[j*BWWidth + i+1] == 0)
				{
					if(Compute(pImag[j*BWWidth + i+1]))
					{
						Graybuff[j*BWWidth + i+1] = 1;
					}
				}

				if(Graybuff[(j+1)*BWWidth + i+1] == 0)
				{
					if(Compute(pImag[(j+1)*BWWidth + i+1]))
					{
						Graybuff[(j+1)*BWWidth + i+1] = 1;
					}
				}

				if(Graybuff[(j+1)*BWWidth + i] == 0)
				{
					if(Compute(pImag[(j+1)*BWWidth + i]))
					{
						Graybuff[(j+1)*BWWidth + i] = 1;
					}
				}
				if(Graybuff[(j+1)*BWWidth + i-1] == 0)
				{
					if(Compute(pImag[(j+1)*BWWidth + i-1]))
					{
						Graybuff[(j+1)*BWWidth + i-1] = 1;
					}
				}
			}
		}
	}

	//从下往上扫描，重新判断
	for(j=BWHeight-2;j>1;j--)
	{
		for(i=BWWidth-2;i>1;i--)
		{
			if(Graybuff[j*BWWidth + i]==1)
			{
				if(Graybuff[j*BWWidth + i-1] == 0)
				{
					if(Compute(pImag[j*BWWidth + i-1]))
					{
						Graybuff[j*BWWidth + i-1] = 1;
					}
				}

				if(Graybuff[(j-1)*BWWidth + i-1] == 0)
				{
					if(Compute(pImag[(j-1)*BWWidth + i-1]))
					{
						Graybuff[(j-1)*BWWidth + i-1] = 1;
					}
				}

				if(Graybuff[(j-1)*BWWidth + i] == 0)
				{
					if(Compute(pImag[(j-1)*BWWidth + i]))
					{
						Graybuff[(j-1)*BWWidth + i] = 1;
					}
				}

				if(Graybuff[(j-1)*BWWidth + i+1] == 0)
				{
					if(Compute(pImag[(j-1)*BWWidth + i+1]))
					{
						Graybuff[(j-1)*BWWidth + i+1] = 1;
					}
				}
			}
		}
	}


	//计算颜色均值
	for(j=0;j<BWHeight-1;j++)
	{
		for(i=0;i<BWWidth-1;i++)
		{
			if(Graybuff[j*BWWidth + i]==1)
			{
				index++;
				r=g=b=0;
				value = pImag[j*BWWidth + i]/256;
				b = pImag[j*BWWidth + i]%256;
				g = value%256;
				r = value/256;
				Exg = Exg + 2*g-r-b;
			}
		}
	}
	return Exg/index;
}

