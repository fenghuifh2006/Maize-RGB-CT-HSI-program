// PathForSingle.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "PathForSingle.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "memory.h"


//�������ṹ��
struct Node;
typedef Node *pNode;

/*��ṹ�嶨��*/
struct Point
{
	unsigned int x;
	unsigned int y;
};

/*���·���˵�*/
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
Function:Expand // ��������
Description:��ͼ��������չһ������ // �������ܡ����ܵȵ�����
Calls:None // �����������õĺ����嵥
Called By:������Ҫ����ͼ����չ�ĺ���������������ص�İ����� // ���ñ������ĺ����嵥
Table Accessed:None // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:None // ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input: // �������˵��������ÿ����������
unsigned char *Array�� ԭʼͼ�����ָ��
unsigned char *pi�� ��չ���ͼ�����ָ��
unsigned int _width����չ���ͼ��Ŀ�
unsigned int _height����չ���ͼ��ĸ�
// �á�ȡֵ˵�����������ϵ��
Output: // �����������˵����
unsigned char *pi�� ��Ϊ��չ����ͼ�����
Return: // ��������ֵ��˵��
bool�� ����ͼ����չ�Ƿ�����
Others: // ����˵��
*************************************************/
bool Expand(unsigned char *Array,unsigned char *pi,unsigned int _width, unsigned int _height)
{
	/*������������*/
	/*ͼ������������*/
	unsigned int x,y;

	/*��ͼ����б�������չ*/
	for (y = 0; y<_height; y++) //ע���ֹԽ��
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
Function:Erode // ��������
Description:����չ���ͼ��ԭ����ȥ������һ�����ص� // �������ܡ����ܵȵ�����
Calls:None // �����������õĺ����嵥
Called By:Expand // ���ñ������ĺ����嵥
Table Accessed:None // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated:None // ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input: // �������˵��������ÿ����������
unsigned char *Array�� ��������һ�����غ�ͼ�����ָ��
unsigned char *pi�� ԭʼ��ͼ�����ָ��
unsigned int _width����չ���ͼ��Ŀ�
unsigned int _height����չ���ͼ��ĸ�
// �á�ȡֵ˵�����������ϵ��
Output: // �����������˵����
unsigned char *Array�� ��������һ�����غ�ͼ�����ָ��
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
bool Erode( unsigned char *Array,unsigned char *pi,
		   unsigned int _width, unsigned int _height )
{
	/*������������*/
	/*ͼ������������*/
	unsigned int x,y;

	/*��ͼ����б�������ԭ*/
	for ( y = 1; y < _height-1; y++)//ע���ֹԽ��
	{
		for (x = 1;x < _width-1; x++)
		{	
			Array[(y-1) * (_width-2) + x-1] = pi[y * _width + x];
		}/*x=_width-1*/
	}/*x=_height-1*/

	return true;
}

/*************************************************
Function: NextJunction // ��������
Description:
����ǰ���ص����ű�ǵ�·���ƽ�����һ�ڵ��˵�// �������ܡ����ܵȵ�����
Calls: // �����������õĺ����嵥
unsigned char *Array, ͼ�����ָ��
unsigned int &x, ������һ�ڵ��˵��x����
unsigned int &y, ������һ�ڵ��˵��y����
unsigned int BWWidth, ͼ��Ŀ��
int &length, ����ԭʼ����+�ƽ����ȵĺ�
Called By: ReadTree// ���ñ������ĺ����嵥
Table Accessed: None // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated: None// ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input: // �������˵��������ÿ����������
unsigned char *Array, ͼ�����ָ��
unsigned int &x, �����صĳ�ʼ��x����
unsigned int &y, �����صĳ�ʼ��y����
unsigned int BWWidth, ͼ�������
int &length,�����صĸ�������һ�ڵ㳤��
// �á�ȡֵ˵�����������ϵ��
Output: // �����������˵����
Return: // ��������ֵ��˵��
�ƽ��Ƿ�����
Others: // ����˵��
*************************************************/

int NextJunction( unsigned char *Array, unsigned int &x,
			unsigned int &y,unsigned int BWWidth,float &length, unsigned int &x0, unsigned int &y0, unsigned int Distance )
{
	/*������������*/
	/*�õ㵽��һ�ڵ㳤�ȱ���*/
	float L = 1;

	/*��ֹ��ѭ������������*/
	int count = 0;

	/*Array[y*BWWidth + x] !=0�Ƿ�ֹ�Ѿ����*/
	while (Array[y*BWWidth + x] != 1 && Array[y*BWWidth + x] != 2 
		  && Array[y*BWWidth + x] !=0)
	{
		int i,j;
		bool flag = false;


		/*ȡ���ߵ�*/
		if(Distance<(unsigned)L && L<Distance+10)
		{
			x0= x;
			y0= y;
		}

		/*���ű�ǵķ��򣬱����ýڵ�����*/
		for (j = -1;j < 2;j++)
		{
			for (i = -1;i < 2;i++)
			{
				/*ȥ������ĵ�*/
				if (i == 0 && j == 0)
				{
					continue;
				}

				/*���ű�ǵķ���*/
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

			/*��������ѭ��*/
			if (flag)
			{
				break;
			}
		}

		/*��ֹ��ѭ��������*/
		if (count++ > 1000) 
		{
			break;
		}
	}/*�ﵽ�������߻�õ�Ϊ�ڵ��˵�*/

	length = length+L;
	return 1;
}

/*������ҶƬ*/
int NextJunction2( unsigned char *Array, unsigned int x,
			unsigned int y,unsigned int BWWidth)
{
	/*������������*/

	/*��ֹ��ѭ������������*/
	int count = 0;

	/*Array[y*BWWidth + x] !=0�Ƿ�ֹ�Ѿ����*/
	while (Array[y*BWWidth + x] != 1 && Array[y*BWWidth + x] != 2 
		  && Array[y*BWWidth + x] !=0)
	{
		int i,j;
		bool flag = false;
		/*���ű�ǵķ��򣬱����ýڵ�����*/
		for (j = -1;j < 2;j++)
		{
			for (i = -1;i < 2;i++)
			{
				/*ȥ������ĵ�*/
				if (i == 0 && j == 0)
				{
					continue;
				}

				/*���ű�ǵķ���*/
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

			/*��������ѭ��*/
			if (flag)
			{
				break;
			}
		}

		/*��ֹ��ѭ��������*/
		if (count++ > 1000) 
		{
			break;
		}
	}/*�ﵽ�������߻�õ�Ϊ�ڵ��˵�*/

	return 1;
}


/*************************************************
Function: // ��������
MaxPath
Description: // �������ܡ����ܵȵ�����
���������ͼ������·������
Calls: // �����������õĺ����嵥
ReaTree������
Expand������
Erode������
Called By: // ���ñ������ĺ����嵥
Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
Table Updated: // ���޸ĵı����������ǣ�������ݿ�����ĳ���
Input: // �������˵��������ÿ����������
unsigned char *Array, ����ǵĹǼ�ͼ�����
unsigned int _BWWidth, ͼ��Ŀ��
unsigned int _BWHeight, ͼ��ĸ߶�
// �á�ȡֵ˵�����������ϵ��
Output: // �����������˵����
Return: // ��������ֵ��˵��
length, ���·������
Others: // ����˵��
*************************************************/
int PathExtra_Single(	unsigned char *Array, // ��ֵͼ
						unsigned int  _BWWidth, //ͼ���
						unsigned int  _BWHeight, //ͼ��߶�
						unsigned int _Xstart, //x��ʼλ��
						unsigned int _Distance, //��������
						float *leaflength,	//ÿƬҶƬ��
						float *OClength,	//ÿƬҶŷʽ����
						double *angle,		//ÿƬҶ�Ƕ�1
						double *angle1,		//ÿƬҶ�Ƕ�2
						float *WanQD,		//ÿƬҶ������
						float &Mean_length, //ƽ��Ҷ����
						float &Mean_OClength, //ƽ��ŷʽ����
						float &Mean_WanQD,	//ƽ��������
						double &Mean_Angle,  //ƽ��Ҷ�Ƕ�
						double &Mean_Angle1,  //ƽ��Ҷ�Ƕ�1
						float &lazhizhugao,  //��ֱ���
						float &total_leaflength, //��Ҷ��
						int &leaf_number	//ҶƬ��
					)//�������
{
	/*������������*/
	/*ͼ������е�x,y����*/
	unsigned int x,y;
	int i,j,k;

	int hight = 0; //��ʼ��ĸ߶�
	float length = 0;

	unsigned int x1=0;
	unsigned int y1=0;
	unsigned int QX_length = 20;

	float L1 = 0; //ҶƬ����
	Node MaxNode;
	int leaf_num=0;

	int Max_x=0;//���������ֱ���ҶƬ
	int Max_y=0;

	/*��չ���ͼ������Ⱥ͸߶�*/
	unsigned int BWWidth = _BWWidth +2;
	unsigned int BWHeight = _BWHeight +2;

	/*��չ���ͼ�����*/
	unsigned char *pArray,*rArray;
	unsigned int BufSize = BWWidth * BWHeight;
	pArray  = new unsigned char[BufSize];
	rArray  = new unsigned char[BufSize];

	/*��ͼ�������չ*/
	if (!Expand( Array,pArray,BWWidth,BWHeight)||!Expand( Array,rArray,BWWidth,BWHeight)) 
	{
		return 1;
	}

	/*�����˵�*/
	for (y = BWHeight-2; y>0; y--)
	{
		for (x = _Xstart; x <_Xstart + _Distance; x++)
		{
			if (pArray[y*BWWidth + x] == 1)
			{
				/*���ö˵�����*/
				pArray[y*BWWidth + x] = 0;

				/*����ڵ�����ռ�*/
				Point *point = new Point[4];
				k = 0;

				/*���ڵ㲻Ϊ��������洢��Point[k]��*/
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
				/*����������3������쳣������*/
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
				leaflength[leaf_num] =L1 ;	//����ҶƬ��
				OClength[leaf_num] = sqrt(float((MaxNode.x-MaxNode.x0)*(MaxNode.x-MaxNode.x0)+(MaxNode.y0 - MaxNode.y)*(MaxNode.y0 - MaxNode.y))); //����ŷʽ����
				WanQD[leaf_num] = OClength[leaf_num]/leaflength[leaf_num]; //����������

				
				//�Ƕ�1
				if(MaxNode.x!=MaxNode.x0)
				{
					double tanvalue= (MaxNode.y0 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x0);
					double arc = atan(tanvalue);//����
					angle1[leaf_num]=arc*180/3.14159;//�Ƕȣ���ע�����������Һ������ƣ�asin,acos
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

				//�Ƕ�2
				if(MaxNode.x!=MaxNode.x1)
				{
					double tanvalue= (MaxNode.y1 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x1);
					double arc = atan(tanvalue);//����
					angle[leaf_num]=arc*180/3.14159;//�Ƕȣ���ע�����������Һ������ƣ�asin,acos
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
				

				total_leaflength+=leaflength[leaf_num]; //����Ҷ����
				Mean_OClength+=OClength[leaf_num]; //���ܵ�OCҶƬ��
				Mean_WanQD+=WanQD[leaf_num]; //���ܵ�������
				Mean_Angle+=fabs(angle[leaf_num]); //���ܵ����߽Ƕ�
				Mean_Angle1+=fabs(angle1[leaf_num]);//���ܵĽǶ�2

				if(MaxNode.length>lazhizhugao)
				{
					lazhizhugao = MaxNode.length;//����ֱ���
					Max_x = MaxNode.x;
					Max_y = MaxNode.y;
				}
				leaf_num++;
				delete[] point;
			}
		}
	}

	//����ƽ��ֵ
	leaf_number=leaf_num; //ҶƬ��
	Mean_length = total_leaflength/leaf_number;
	Mean_OClength = Mean_OClength/leaf_number;
	Mean_WanQD = Mean_WanQD/leaf_number;
	Mean_Angle = Mean_Angle/leaf_number;
	Mean_Angle1 = Mean_Angle1/leaf_number;

	/*������ҶƬ*/
	rArray[Max_y*BWWidth +Max_x] = 0;
	/*���ڵ㲻Ϊ��������洢��Point[k]��*/
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

	/*�ָ�ͼ��*/
	if (!Erode( Array,rArray,BWWidth,BWHeight ))
	{
		return 1;
	}

	/*�ͷ���չͼ�����ռ�*/
	delete pArray;
	delete rArray;
	/*������*/
	return	1;
}

