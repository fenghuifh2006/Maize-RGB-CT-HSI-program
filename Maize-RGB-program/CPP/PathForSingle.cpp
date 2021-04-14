// PathForSingle.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "PathForSingle.h"
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
	int x1;
	int y1;
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

int NextJunction( unsigned char *Array, unsigned int &x,
			unsigned int &y,unsigned int BWWidth,float &length, unsigned int &x0, unsigned int &y0, unsigned int Distance )
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


		/*取切线点*/
		if(Distance<(unsigned)L && L<Distance+10)
		{
			x0= x;
			y0= y;
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
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L++;
					flag = true;
					break;
				}
				else if (Array[(y+j)*BWWidth+x+i] == Array[y*BWWidth + x] && abs(i)+abs(j) == 2)
				{	
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L += (float)1.414;
					flag = true;
					break;
				}
				else if (Array[(y+j)*BWWidth+x+i] ==1 || Array[(y+j)*BWWidth+x+i] == 2)
				{	
					Array[y*BWWidth + x] = 0;
					x = x + i;
					y = y + j;
					L++;
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
int PathExtra_Single(	unsigned char *Array, // 二值图
						unsigned int  _BWWidth, //图像宽
						unsigned int  _BWHeight, //图像高度
						unsigned int _Xstart, //x开始位置
						unsigned int _Distance, //搜索长度
						float *leaflength,	//每片叶片长
						float *OClength,	//每片叶欧式距离
						double *angle,		//每片叶角度1
						double *angle1,		//每片叶角度2
						float *WanQD,		//每片叶弯曲度
						float &Mean_length, //平均叶长；
						float &Mean_OClength, //平均欧式距离
						float &Mean_WanQD,	//平均弯曲度
						double &Mean_Angle,  //平均叶角度
						double &Mean_Angle1,  //平均叶角度1
						float &lazhizhugao,  //拉直株高
						float &total_leaflength, //总叶长
						int &leaf_number	//叶片数
					)//计算参数
{
	/*函数变量定义*/
	/*图像矩阵中的x,y坐标*/
	unsigned int x,y;
	int i,j,k;

	int hight = 0; //初始点的高度
	float length = 0;

	unsigned int x1=0;
	unsigned int y1=0;
	unsigned int QX_length = 20;

	float L1 = 0; //叶片长度
	Node MaxNode;
	int leaf_num=0;

	int Max_x=0;//定义最大拉直株高叶片
	int Max_y=0;

	/*扩展后的图像矩阵宽度和高度*/
	unsigned int BWWidth = _BWWidth +2;
	unsigned int BWHeight = _BWHeight +2;

	/*扩展后的图像矩阵*/
	unsigned char *pArray,*rArray;
	unsigned int BufSize = BWWidth * BWHeight;
	pArray  = new unsigned char[BufSize];
	rArray  = new unsigned char[BufSize];

	/*对图像进行扩展*/
	if (!Expand( Array,pArray,BWWidth,BWHeight)||!Expand( Array,rArray,BWWidth,BWHeight)) 
	{
		return 1;
	}

	/*搜索端点*/
	for (y = BWHeight-2; y>0; y--)
	{
		for (x = _Xstart; x <_Xstart + _Distance; x++)
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
				NextJunction( pArray,point[0].x,point[0].y, BWWidth,L1,x1,y1,QX_length);
				
				MaxNode.x = x;
				MaxNode.y = y;
				MaxNode.x0 = point[0].x;
				MaxNode.y0 = point[0].y;
				MaxNode.x1 = x1;
				MaxNode.y1 = y1;
				MaxNode.length =  L1+BWHeight-MaxNode.y;
				leaflength[leaf_num] =L1 ;	//计算叶片长
				OClength[leaf_num] = sqrt(float((MaxNode.x-MaxNode.x0)*(MaxNode.x-MaxNode.x0)+(MaxNode.y0 - MaxNode.y)*(MaxNode.y0 - MaxNode.y))); //计算欧式距离
				WanQD[leaf_num] = OClength[leaf_num]/leaflength[leaf_num]; //计算弯曲度

				
				//角度1
				if(MaxNode.x!=MaxNode.x0)
				{
					double tanvalue= (MaxNode.y0 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x0);
					double arc = atan(tanvalue);//弧度
					angle1[leaf_num]=arc*180/3.14159;//角度，另注：反正弦余弦函数类似，asin,acos
					if(MaxNode.x-MaxNode.x0 < 0)
					{
						angle1[leaf_num] =90 - angle1[leaf_num];
					}											
					else if(MaxNode.x-MaxNode.x0 >= 0)
					{
						angle1[leaf_num] =(angle1[leaf_num] + 90)*(-1.0);
					}
				}
				else
				{
					angle1[leaf_num]  = 0;
				}

				//角度2
				if(MaxNode.x!=MaxNode.x1)
				{
					double tanvalue= (MaxNode.y1 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x1);
					double arc = atan(tanvalue);//弧度
					angle[leaf_num]=arc*180/3.14159;//角度，另注：反正弦余弦函数类似，asin,acos
					if(MaxNode.x-MaxNode.x1 < 0)
					{
						angle[leaf_num] =90 - angle[leaf_num];
					}
					else if(MaxNode.x-MaxNode.x1 >0)
					{
						angle[leaf_num] =(angle[leaf_num] + 90)*(-1.0);
					}
				}
				else
				{
					angle[leaf_num] = 0;
				}
				

				total_leaflength+=leaflength[leaf_num]; //求总叶长；
				Mean_OClength+=OClength[leaf_num]; //求总的OC叶片长
				Mean_WanQD+=WanQD[leaf_num]; //求总的弯曲度
				Mean_Angle+=fabs(angle[leaf_num]); //求总的切线角度
				Mean_Angle1+=fabs(angle1[leaf_num]);//求总的角度2

				if(MaxNode.length>lazhizhugao)
				{
					lazhizhugao = MaxNode.length;//求拉直株高
					Max_x = MaxNode.x;
					Max_y = MaxNode.y;
				}
				leaf_num++;
				delete[] point;
			}
		}
	}

	//计算平均值
	leaf_number=leaf_num; //叶片数
	Mean_length = total_leaflength/leaf_number;
	Mean_OClength = Mean_OClength/leaf_number;
	Mean_WanQD = Mean_WanQD/leaf_number;
	Mean_Angle = Mean_Angle/leaf_number;
	Mean_Angle1 = Mean_Angle1/leaf_number;

	/*标记最大叶片*/
	rArray[Max_y*BWWidth +Max_x] = 0;
	/*将节点不为零的领域点存储在Point[k]中*/
	for (j= -1;j<2;j++)
	{
		for (i=-1;i<2;i++)
		{
			if (rArray[(Max_y+j)*BWWidth+ Max_x+i])
			{	
				x = Max_x+i;
				y = Max_y+j;
			}
		}
	}
	NextJunction2( rArray,x,y, BWWidth);

	/*恢复图像*/
	if (!Erode( Array,rArray,BWWidth,BWHeight ))
	{
		return 1;
	}

	/*释放扩展图像矩阵空间*/
	delete pArray;
	delete rArray;
	/*返回真*/
	return	1;
}

