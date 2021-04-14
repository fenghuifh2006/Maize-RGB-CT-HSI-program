// PathExtra.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "PathExtra.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "memory.h"


//二叉树结构体
struct Node;
typedef Node *pNode;

/*点结构体定义*/
struct Point
{
	unsigned int x;
	unsigned int y;
};

/*最大路径端点*/
struct Node
{
	int x;
	int y;
	int x0;
	int y0;
	float length;
};
/*************************************************
Function:Expand // 函数名称
Description:对图像四周扩展一个像素 // 函数功能、性能等的描述
Calls:None // 被本函数调用的函数清单
Called By:所有需要进行图像扩展的函数（如需访问像素点的八邻域） // 调用本函数的函数清单
Table Accessed:None // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:None // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作
unsigned char *Array， 原始图像矩阵指针
unsigned char *pi， 扩展后的图像矩阵指针
unsigned int _width，扩展后的图像的宽
unsigned int _height，扩展后的图像的高
// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
unsigned char *pi， 作为扩展后后的图像输出
Return: // 函数返回值的说明
bool， 返回图像扩展是否正常
Others: // 其它说明
*************************************************/
bool Expand(unsigned char *Array,unsigned char *pi,unsigned int _width, unsigned int _height)
{
	/*函数变量定义*/
	/*图像矩阵横纵坐标*/
	unsigned int x,y;

	/*对图像进行遍历，扩展*/
	for (y = 0; y<_height; y++) //注意防止越界
	{
		for (x = 0; x < _width; x++)
		{
			if (x == 0 || x == _width-1 || y == 0 || y == _height-1)
			{
				pi[y * _width + x] = 0;
			}
			else
			{
				pi[y * _width + x] = Array[(y-1) * (_width-2) + x-1];
			}
		}/*x=_width*/
	}/*x=_height*/

	return true;
}

/*************************************************
Function:Erode // 函数名称
Description:将扩展后的图像还原，即去除四周一行像素点 // 函数功能、性能等的描述
Calls:None // 被本函数调用的函数清单
Called By:Expand // 调用本函数的函数清单
Table Accessed:None // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated:None // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作
unsigned char *Array， 消除四周一行像素后图像矩阵指针
unsigned char *pi， 原始的图像矩阵指针
unsigned int _width，扩展后的图像的宽
unsigned int _height，扩展后的图像的高
// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
unsigned char *Array， 消除四周一行像素后图像矩阵指针
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
bool Erode( unsigned char *Array,unsigned char *pi,
		   unsigned int _width, unsigned int _height )
{
	/*函数变量定义*/
	/*图像矩阵横纵坐标*/
	unsigned int x,y;

	/*对图像进行遍历，还原*/
	for ( y = 1; y < _height-1; y++)//注意防止越界
	{
		for (x = 1;x < _width-1; x++)
		{	
			Array[(y-1) * (_width-2) + x-1] = pi[y * _width + x];
		}/*x=_width-1*/
	}/*x=_height-1*/

	return true;
}

/*************************************************
Function: NextJunction // 函数名称
Description:
将当前像素点沿着标记的路径推进至下一节点或端点// 函数功能、性能等的描述
Calls: // 被本函数调用的函数清单
unsigned char *Array, 图像矩阵指针
unsigned int &x, 返回下一节点或端点的x坐标
unsigned int &y, 返回下一节点或端点的y坐标
unsigned int BWWidth, 图像的宽度
int &length, 返回原始长度+推进长度的和
Called By: ReadTree// 调用本函数的函数清单
Table Accessed: None // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated: None// 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作
unsigned char *Array, 图像矩阵指针
unsigned int &x, 带返回的初始点x坐标
unsigned int &y, 带返回的初始点y坐标
unsigned int BWWidth, 图像矩阵宽度
int &length,带返回的根部到下一节点长度
// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
推进是否正常
Others: // 其它说明
*************************************************/

int NextJunction( unsigned char *Array,unsigned char *fArray, unsigned int &x,
			unsigned int &y,unsigned int BWWidth,float &length )
{
	/*函数变量定义*/
	/*该点到下一节点长度变量*/
	float L = 1;

	/*防止死循环，计数变量*/
	int count = 0;

	/*Array[y*BWWidth + x] !=0是防止已经标记*/
	while (Array[y*BWWidth + x] != 1 && Array[y*BWWidth + x] != 2 
		  && Array[y*BWWidth + x] !=0)
	{
		int i,j;
		bool flag = false;

		/*碰到端点、结点停止*/
		for (j = -1;j < 2;j++)
		{
			for (i = -1;i < 2;i++)
			{
				/*去除本身的点*/
				if (i == 0 && j == 0)
				{
					continue;
				}
				else if (Array[(y+j)*BWWidth+x+i] ==1 || Array[(y+j)*BWWidth+x+i] == 2)
				{
					fArray[y*BWWidth + x] = 250;
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L++;
					flag = true;
					break;
				}
			}
		}
		if (flag)
			{
				flag = false;
				break;
			}
		/*沿着标记的方向，遍历该节点邻域*/
		for (j = -1;j < 2;j++)
		{
			for (i = -1;i < 2;i++)
			{
				/*去除本身的点*/
				if (i == 0 && j == 0)
				{
					continue;
				}

				/*沿着标记的方向*/
				else if (Array[(y+j)*BWWidth+x+i] == Array[y*BWWidth + x] && abs(i+j) == 1)
				{	
					fArray[y*BWWidth + x] = 250;
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L++;
					flag = true;
					break;
				}
				else if (Array[(y+j)*BWWidth+x+i] == Array[y*BWWidth + x] && abs(i)+abs(j) == 2)
				{	
					fArray[y*BWWidth + x] = 250;
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L += (float)1.414;
					flag = true;
					break;
				}
				/*else if (Array[(y+j)*BWWidth+x+i] ==1 || Array[(y+j)*BWWidth+x+i] == 2)
				{
					fArray[y*BWWidth + x] = 250;
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L++;
					flag = true;
					break;
				}*/
			}

			/*跳出两重循环*/
			if (flag)
			{
				break;
			}
		}

		/*防止死循环，计数*/
		if (count++ > 1000) 
		{
			break;
		}
	}/*达到计数上线或该点为节点或端点*/

	length = length+L;
	return 1;
}

/*标记最大叶片*/
int NextJunction2( unsigned char *Array, unsigned int x,
			unsigned int y,unsigned int BWWidth)
{
	/*函数变量定义*/

	/*防止死循环，计数变量*/
	int count = 0;

	/*Array[y*BWWidth + x] !=0是防止已经标记*/
	while (Array[y*BWWidth + x] != 1 && Array[y*BWWidth + x] != 2 
		  && Array[y*BWWidth + x] !=0)
	{
		int i,j;
		bool flag = false;
		/*沿着标记的方向，遍历该节点邻域*/
		for (j = -1;j < 2;j++)
		{
			for (i = -1;i < 2;i++)
			{
				/*去除本身的点*/
				if (i == 0 && j == 0)
				{
					continue;
				}

				/*沿着标记的方向*/
				else if (Array[(y+j)*BWWidth+x+i] == Array[y*BWWidth + x] && abs(i+j) == 1)
				{	
					Array[y*BWWidth + x] = 255;
					x = x + i;
					y = y + j;
					flag = true;
					
					break;
				}
				else if (Array[(y+j)*BWWidth+x+i] == Array[y*BWWidth + x] && abs(i)+abs(j) == 2)
				{	
					Array[y*BWWidth + x] = 255;
					x = x + i;
					y = y + j;
					flag = true;
					break;
				}
				else if (Array[(y+j)*BWWidth+x+i] ==1 || Array[(y+j)*BWWidth+x+i] == 2)
				{	
					Array[y*BWWidth + x] = 255;
					x = x + i;
					y = y + j;
					flag = true;
					break;
				}
			}

			/*跳出两重循环*/
			if (flag)
			{
				break;
			}
		}

		/*防止死循环，计数*/
		if (count++ > 1000) 
		{
			break;
		}
	}/*达到计数上线或该点为节点或端点*/

	return 1;
}


/*************************************************
Function: // 函数名称
MaxPath
Description: // 函数功能、性能等的描述
搜索被标记图像的最大路径长度
Calls: // 被本函数调用的函数清单
ReaTree（）；
Expand（）；
Erode（）；
Called By: // 调用本函数的函数清单
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated: // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作
unsigned char *Array, 被标记的骨架图像矩阵
unsigned int _BWWidth, 图像的宽度
unsigned int _BWHeight, 图像的高度
// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
length, 最大路径长度
Others: // 其它说明
*************************************************/
int PathExtra_forleaf( unsigned char *Array,unsigned int _BWWidth,unsigned int _BWHeight, unsigned int &x0,
			unsigned int &y0, float &leaflength, float &angle, float &angle1, float &lengthMax, float &OClength)
{
	/*函数变量定义*/
	/*图像矩阵中的x,y坐标*/
	unsigned int x,y;
	int i,j,k;
	float length = 0;
	float L1 = 0;
	OClength=0;
	Node MaxNode;

	/*扩展后的图像矩阵宽度和高度*/
	unsigned int BWWidth = _BWWidth +2;
	unsigned int BWHeight = _BWHeight +2;

	/*扩展后的图像矩阵*/
	unsigned char *pArray,*rArray,*fArray;
	unsigned int BufSize = BWWidth * BWHeight;
	pArray  = new unsigned char[BufSize];
	rArray  = new unsigned char[BufSize];
	fArray  = new unsigned char[BufSize];

	/*对图像进行扩展*/
	if (!Expand( Array,pArray,BWWidth,BWHeight)||!Expand( Array,rArray,BWWidth,BWHeight)||!Expand( Array,fArray,BWWidth,BWHeight)) 
	{
		return 1;
	}
	/*搜索茎秆起始点*/
	bool flags = false;
	for (y = 1; y < BWHeight-1 ; y++)
	{
		for (x = 1; x < BWWidth-1; x++)
		{
			if (fArray[y*BWWidth + x] == 2)
			{
				fArray[y*BWWidth + x]=200;
				flags = true;
				MaxNode.x = x;
				MaxNode.y = y;
				break;
			}
		}
		if(flags)
		{
			flags = false;
			break;

		}
	}
	/*搜索茎秆终点*/
	for (y =BWHeight-1; y > 0 ; y--)
	{
		for (x = 1; x < BWWidth-1; x++)
		{
			if (fArray[y*BWWidth + x] == 1)
			{
				fArray[y*BWWidth + x] = 200;
				pArray[y*BWWidth + x] = 0;
				flags = true;
				MaxNode.x0 = x;
				MaxNode.y0 = y;
				break;
			}
		}
		if(flags)
		{
			flags = false;
			break;
		}
	}


	/*搜索端点*/
	for (y = 1; y < BWHeight-1 ; y++)
	{
		for (x = 1; x < BWWidth-1; x++)
		{
			if (pArray[y*BWWidth + x] == 1)
			{
				/*将该端点置零*/
				pArray[y*BWWidth + x] = 0;

				/*申请节点领域空间*/
				Point *point = new Point[4];
				k = 0;

				/*将节点不为零的领域点存储在Point[k]中*/
				for (j= -1;j<2;j++)
				{
					for (i=-1;i<2;i++)
					{
						if (pArray[(y+j)*BWWidth+x+i])
						{	
							point[k].x = x+i;
							point[k].y = y+j;
							k++;
						}
					}
				}
				/*若领域点大于3则表明异常，返回*/
				if (k>1 || k==0)
				{
					delete[] point;
					continue;
				}
				L1 = 1;
				NextJunction( pArray,fArray,point[0].x,point[0].y, BWWidth,L1);
				if(L1+BWHeight-point[0].y>length)
				{
					length = L1+BWHeight-point[0].y;//找最长的植株的叶片
					/*MaxNode.length = length;
					MaxNode.x = x;
					MaxNode.y = y;
					MaxNode.x0 = point[0].x;
					MaxNode.y0 = point[0].y;*/
				}
				delete[] point;
			}
		}
	}


	/*标记最大叶片*/
	rArray[MaxNode.y*BWWidth + MaxNode.x] = 0;
	/*将节点不为零的领域点存储在Point[k]中*/
	/*for (j= -1;j<2;j++)
	{
		for (i=-1;i<2;i++)
		{
			if (rArray[(MaxNode.y+j)*BWWidth+ MaxNode.x+i])
			{	
				x = MaxNode.x+i;
				y = MaxNode.y+j;
			}
		}
	}
	NextJunction2( rArray,x,y, BWWidth);*/




	/*参数计算*/
	int Len1,Len2, temx,temy;
	Len1 = Len2 = temx = temy =0;

	if(MaxNode.x<(int)BWWidth-MaxNode.x)
	{
		Len1 =MaxNode.x;
		if(MaxNode.y < Len1)
		{
			Len1 = MaxNode.y;
		}
	}
	else
	{
		Len1 =(int)BWWidth-MaxNode.x;
		if(MaxNode.y < Len1)
		{
			Len1 = MaxNode.y;
		}
	}
	if(MaxNode.x0<(int)BWWidth-MaxNode.x0)
	{
		Len2 =MaxNode.x0;
		if(MaxNode.y0 < Len2)
		{
			Len2 = MaxNode.y0;
		}
	}
	else
	{
		Len2 =(int)BWWidth-MaxNode.x0;
		if(MaxNode.y0 < Len2)
		{
			Len2 = MaxNode.y0;
		}
	}
	if(Len1>Len2)
	{
		temx = MaxNode.x;
		MaxNode.x = MaxNode.x0;
		MaxNode.x0 = temx;
		temy = MaxNode.y;
		MaxNode.y = MaxNode.y0;
		MaxNode.y0 = temy;
	}

	x0 = MaxNode.x0;
	y0 = MaxNode.y0;
	//lengthMax= MaxNode.length;
	//leaflength = MaxNode.length - (BWHeight - y0);
	//lengthMax = leaflength +  BWHeight - MaxNode.y0;
	//欧式距离
	OClength=sqrt(float((MaxNode.x-MaxNode.x0)*(MaxNode.x-MaxNode.x0)+(MaxNode.y0 - MaxNode.y)*(MaxNode.y0 - MaxNode.y)));

	//角度1
	if(MaxNode.x!=MaxNode.x0)
	{
		double tanvalue= (MaxNode.y0 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x0);
		double arc = atan(tanvalue);//弧度
		angle1=arc*180/3.14159;//角度，另注：反正弦余弦函数类似，asin,acos
		if(MaxNode.x-MaxNode.x0 < 0)
		{
			//angle1 =angle1 + 90;
		}
		else if(MaxNode.x-MaxNode.x0 >= 0)
		{
			angle1 =180 + angle1;
			//angle1 =90 - angle1;
		}
	}
	else
	{
		angle1 = 0;
	}

	//角度2
	/*bool flag2=false;
	for (y = 1; y < BWHeight-1 ; y++)
	{
		for (x = 1; x < BWWidth-1; x++)
		{
			if(rArray[y*BWWidth + x] == 255)
			{
				float L2=sqrt(float((x-MaxNode.x0)*(x-MaxNode.x0)+(y-MaxNode.y0)*(y-MaxNode.y0)));
				if(L2>30 && L2<40)
				{
					flag2=true;
					MaxNode.x=x;
					MaxNode.y=y;
					break;
				}
			}
		}
		if(flag2)
		{
			break;
		}
	}
	
	//
	if(MaxNode.x!=MaxNode.x0)
	{
		double tanvalue= (MaxNode.y0 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x0);
		double arc = atan(tanvalue);//弧度
		angle=arc*180/3.14159;//角度，另注：反正弦余弦函数类似，asin,acos
		if(MaxNode.x-MaxNode.x0 < 0)
		{
			angle =angle + 90;
		}
		else if(MaxNode.x-MaxNode.x0 >= 0)
		{
			angle =90 - angle;
		}
	}
	else
	{
		angle = 0;
	}*/

	/*恢复图像
	if (!Erode( Array,rArray,BWWidth,BWHeight ))
	{
		return 1;
	}*/
	/*预处理恢复图像*/
	if (!Erode( Array,fArray,BWWidth,BWHeight))
	{
		return 1;
	}

	/*释放扩展图像矩阵空间*/
	delete pArray;
	delete rArray;
	delete fArray;
	/*返回高度*/
	return	BWHeight - y0;
}