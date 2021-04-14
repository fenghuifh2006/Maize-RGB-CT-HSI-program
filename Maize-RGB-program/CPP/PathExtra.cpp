// PathExtra.cpp : ���� DLL Ӧ�ó���ĵ���������
//
#include "stdafx.h"
#include "PathExtra.h"
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

int NextJunction( unsigned char *Array,unsigned char *fArray, unsigned int &x,
			unsigned int &y,unsigned int BWWidth,float &length )
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

		/*�����˵㡢���ֹͣ*/
		for (j = -1;j < 2;j++)
		{
			for (i = -1;i < 2;i++)
			{
				/*ȥ������ĵ�*/
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
int PathExtra_forleaf( unsigned char *Array,unsigned int _BWWidth,unsigned int _BWHeight, unsigned int &x0,
			unsigned int &y0, float &leaflength, float &angle, float &angle1, float &lengthMax, float &OClength)
{
	/*������������*/
	/*ͼ������е�x,y����*/
	unsigned int x,y;
	int i,j,k;
	float length = 0;
	float L1 = 0;
	OClength=0;
	Node MaxNode;

	/*��չ���ͼ������Ⱥ͸߶�*/
	unsigned int BWWidth = _BWWidth +2;
	unsigned int BWHeight = _BWHeight +2;

	/*��չ���ͼ�����*/
	unsigned char *pArray,*rArray,*fArray;
	unsigned int BufSize = BWWidth * BWHeight;
	pArray  = new unsigned char[BufSize];
	rArray  = new unsigned char[BufSize];
	fArray  = new unsigned char[BufSize];

	/*��ͼ�������չ*/
	if (!Expand( Array,pArray,BWWidth,BWHeight)||!Expand( Array,rArray,BWWidth,BWHeight)||!Expand( Array,fArray,BWWidth,BWHeight)) 
	{
		return 1;
	}
	/*����������ʼ��*/
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
	/*���������յ�*/
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


	/*�����˵�*/
	for (y = 1; y < BWHeight-1 ; y++)
	{
		for (x = 1; x < BWWidth-1; x++)
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
				NextJunction( pArray,fArray,point[0].x,point[0].y, BWWidth,L1);
				if(L1+BWHeight-point[0].y>length)
				{
					length = L1+BWHeight-point[0].y;//�����ֲ���ҶƬ
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


	/*������ҶƬ*/
	rArray[MaxNode.y*BWWidth + MaxNode.x] = 0;
	/*���ڵ㲻Ϊ��������洢��Point[k]��*/
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




	/*��������*/
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
	//ŷʽ����
	OClength=sqrt(float((MaxNode.x-MaxNode.x0)*(MaxNode.x-MaxNode.x0)+(MaxNode.y0 - MaxNode.y)*(MaxNode.y0 - MaxNode.y)));

	//�Ƕ�1
	if(MaxNode.x!=MaxNode.x0)
	{
		double tanvalue= (MaxNode.y0 - MaxNode.y)*(1.0)/(MaxNode.x-MaxNode.x0);
		double arc = atan(tanvalue);//����
		angle1=arc*180/3.14159;//�Ƕȣ���ע�����������Һ������ƣ�asin,acos
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

	//�Ƕ�2
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
		double arc = atan(tanvalue);//����
		angle=arc*180/3.14159;//�Ƕȣ���ע�����������Һ������ƣ�asin,acos
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

	/*�ָ�ͼ��
	if (!Erode( Array,rArray,BWWidth,BWHeight ))
	{
		return 1;
	}*/
	/*Ԥ����ָ�ͼ��*/
	if (!Erode( Array,fArray,BWWidth,BWHeight))
	{
		return 1;
	}

	/*�ͷ���չͼ�����ռ�*/
	delete pArray;
	delete rArray;
	delete fArray;
	/*���ظ߶�*/
	return	BWHeight - y0;
}