#define DLL_API extern "C" _declspec(dllexport)
//#include<windows.h>
#include<stdio.h>
#include <math.h>
#include "stdlib.h"

#define STACK_INIT_SIZE 1000//�洢�ռ��ʼ������
#define STACKINCREMENT 100//�洢�ռ��������

typedef struct
{
	int *base;//ջ��ָ��
	int *top;//ջ��ָ��
	int stacksize;//��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
}Stack;

//����һ����ջS
bool InitStack(Stack &S)
{
	S.base =(int *) malloc(STACK_INIT_SIZE * sizeof(int));
	if(!S.base)
		return false;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return true;
}

//��ջ������Ԫ��eΪ�µ�ջ��Ԫ��
bool Push(Stack &S, int e)
{
	if(S.top - S.base >= S.stacksize)//ջ����׷�Ӵ洢�ռ�
	{
		S.base = (int*) realloc (S.base, (S.stacksize + STACKINCREMENT) * sizeof(int));
		if(!S.base) return false;//�洢����ʧ��
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return true;
}

//��ջ����ջ���գ�ɾ��S��ջ��Ԫ�أ���e������ֵ
int Pop(Stack &S)
{
	if(S.top == S.base) return false;
	//*e = * --S.top;
	return * --S.top;
}
//��ջSΪ��ջ������1�����򷵻�0
bool StackEmpty(Stack S)
{
	return S.top == S.base;
}

//����ջS
void DestroyStack(Stack &S)
{
	delete []S.base;
	S.base = NULL;
	S.top = NULL;
}



DLL_API int imbinary(float *im,
					 int row,
					 int col,
					 float level,
					 unsigned char *bw);
DLL_API int imbinary(float *im,
					 int row,
					 int col,
					 float level,
					 unsigned char *bw)
{
	int i, size;
	size = row * col;
	for ( i = 0; i < size; i++ )
		bw[i] = im[i] >= level ? 1 : 0;
	return 1;
}





DLL_API	int bwfill(unsigned char *im,
				   int row,
				   int col);
DLL_API	int bwfill(unsigned char *im,
				   int row,
				   int col)
{
	int size = row * col;
	int i,j,e,ee;
	Stack S;
	if(!InitStack(S)) return 0;

	for (j = 0; j < col; j++)
	{
		im[j] = 2;
		im[(row-1)*col+j] = 2;
	}
	for(i = 0; i < row; i++)
	{
		im[i*col] = 2;
		im[i*col+col-1] = 2;
	}

	e = col + 1;
	Push(S,e);
	im[e] = 2;

	while(!StackEmpty(S))
	{
		e = Pop(S);
		ee = e  - 1;
		if( im[ee]==0 )
		{
			if(!Push(S,ee))
				return 0;
			im[ee] = 2;
		}
		ee = e  + 1;
		if( im[ee]==0 )
		{
			if(!Push(S,ee))
				return 0;
			im[ee] = 2;
		}
		ee = e  - col;
		if( im[ee]==0 )
		{
			if(!Push(S,ee))
				return 0;
			im[ee] = 2;
		}
		ee = e  + col;
		if( im[ee]==0 )
		{
			if(!Push(S,ee))
				return 0;
			im[ee] = 2;
		}


	}
	DestroyStack(S);

	for(j = 0; j < col; j++)
		for(i = 0; i < row; i++)
		{
			if(im[i*col+j] == 2)
				im[i*col+j] = 0;
			else if(im[i*col+j] == 0)
				im[i*col+j] = 1;
		}

		return 1;

}


DLL_API	int bwremove(unsigned char *im,
					 int row,
					 int col,
					 int AreaThreshold);
DLL_API	int bwremove(unsigned char *im,
					 int row,
					 int col,
					 int AreaThreshold)
{
	row += 2;
	col += 2;
	int size = row * col; //���ӱ߽�
	int i,j,k,m,n,e,ee,count;
	//int number = 0;
	Stack S;
	if(!InitStack(S)) return 0;
	if (AreaThreshold==0) return 1;
	unsigned char *imlabel = new unsigned char[size];
	unsigned char *imcopy = new unsigned char[size];
	int *remove = new int[AreaThreshold];

	for(i = 0; i < size; i++)
	{
		imlabel[i] = 0;
		imcopy[i] = 0;
	}

	for(i = 0; i < AreaThreshold; i++)
		remove[i] = 0;

	for ( i = 1; i < row-1; i++ )
		for ( j = 1; j < col-1; j++ )
		{
			e = i * col + j;
			ee = (i-1) * (col-2) + j - 1;
			imcopy[e] = im[ee];
		}

	for(i = 1; i < row-1; i++)
		for(j = 1; j < col-1; j++)
		{
			e = i * col + j;
			if( imcopy[e]!=0 && imlabel[e]==0 )
			{
				remove[0] = e;
				count = 0;
				//number++;
				imlabel[e] = 1;
				if(!Push(S,e))
					return 0;
				while(!StackEmpty(S))
				{
					e = Pop(S);
					for(m = -1; m <= 1; m++)
						for(n = -1; n <= 1;n++)
						{
							ee = e + m * col + n;
							if( imcopy[ee]!=0 && imlabel[ee]==0 )
							{
								if(!Push(S,ee))
									return 0;
								imlabel[ee] = 1;
								count++;
								if(count < AreaThreshold)
									remove[count] = ee;
							}
						}
				}
				if( count < AreaThreshold)
				{
					for(k = 0; k <= count; k++)
						imlabel[remove[k]] = 2;
				}
			}
		}

		for ( i = 1; i < row-1; i++ )
			for ( j = 1; j < col-1; j++ )
			{
				e = i * col + j;
				if(imlabel[e]==2)
				{
					ee = (i-1) * (col-2) + j - 1;
					im[ee] = 0;
				}
			}
		
		DestroyStack(S);
		delete []imlabel;
		delete []remove;
		delete []imcopy;
		return 1;
}

DLL_API	int bwremove1(unsigned char *im,
					 int row,
					 int col,
					 int AreaThreshold);
DLL_API	int bwremove1(unsigned char *im,
					 int row,
					 int col,
					 int AreaThreshold)
{
	int size = row * col;
	int i,j,k,m,n,e,ee,count;
	//int number = 0;
	Stack S;
	if(!InitStack(S)) return 0;
	if (AreaThreshold==0) return 1;
	unsigned char *imlabel = new unsigned char[size];
	int *remove = new int[AreaThreshold];
	for(i = 0; i < size; i++)
		imlabel[i] = 0;
	for(i = 0; i < AreaThreshold; i++)
		remove[i] = 0;
	for(i = 1; i < row-1; i++)
		for(j = 1; j < col-1; j++)
		{
			e = i * col + j;
			if( im[e]!=0 && imlabel[e]==0 )
			{
				remove[0] = e;
				count = 0;
				//number++;
				imlabel[e] = 1;
				if(!Push(S,e))
					return 0;
				while(!StackEmpty(S))
				{
					e = Pop(S);
					for(m = -1; m <= 1; m++)
						for(n = -1; n <= 1;n++)
						{
							ee = e + m * col + n;
							if( im[ee]!=0 && imlabel[ee]==0 )
							{
								if(!Push(S,ee))
									return 0;
								imlabel[ee] = 1;
								count++;
								if(count < AreaThreshold)
									remove[count] = ee;
							}
						}
				}
				if( count < AreaThreshold)
				{
					for(k = 0; k <= count; k++)
						imlabel[remove[k]] = 2;
				}
			}
		}
		for(i = 0; i < size; i++)
		{
			if(imlabel[i]==2)
				im[i] = 0;
			//im[i] = imlabel[i];
		}
		DestroyStack(S);
		delete []imlabel;
		delete []remove;
		return 1;
}








DLL_API	int bwFillHole(unsigned char *im,
					   int row,
					   int col,
					   int AreaThreshold);
DLL_API	int bwFillHole(unsigned char *im,
					   int row,
					   int col,
					   int AreaThreshold)
{
	row += 2;
	col += 2;
	int size = row * col;
	int i,j,k,m,n,e,ee,count;
	//int number = 0;
	Stack S;
	if(!InitStack(S)) return 0;
	if (AreaThreshold==0) return 1;
	unsigned char *imlabel = new unsigned char[size];
	unsigned char *imcopy = new unsigned char[size];

	for(i = 0; i < size; i++)
	{
		imlabel[i] = 0;
		imcopy[i] = 0;
	}

	for (j = 0; j < col; j++)  //�߽���1,����Ϊǰ����
	{
		imcopy[j] = 1;
		imcopy[(row-1)*col+j] = 1;
	}
	for(i = 0; i < row; i++)
	{
		imcopy[i*col] = 1;
		imcopy[i*col+col-1] = 1;
	}

	for ( i = 1; i < row-1 ; i++ )
		for ( j = 1; j < col-1; j++)
		{
			e = i * col + j;
			ee = (i-1) * (col-2) + j - 1;
			imcopy[e] = im[ee];
		}

	int *fill = new int[AreaThreshold];//����Ҫ���������ֵ	

	for(i = 0; i < AreaThreshold; i++)
		fill[i] = 0;

	for(i = 1; i < row-1; i++)
		for(j = 1; j < col-1; j++)
		{
			e = i * col + j;
			if( imcopy[e]==0 && imlabel[e]==0 )
			{
				fill[0] = e;
				count = 0;
				//number++;
				imlabel[e] = 1;
				if(!Push(S,e))
					return 0;
				while(!StackEmpty(S))
				{
					e = Pop(S);
					for(m = -1; m <= 1; m++)
						for(n = -1; n <= 1;n++)
						{
							if ( m*n==0 )
							{
								ee = e + m * col + n;
								if( imcopy[ee]==0 && imlabel[ee]==0 )
								{
									if(!Push(S,ee))
										return 0;
									imlabel[ee] = 1;
									count++;
									if(count < AreaThreshold)
										fill[count] = ee;
								}
							}
							
						}
				}
				if( count < AreaThreshold)
				{
					for(k = 0; k <= count; k++)
						imlabel[fill[k]] = 2;
				}
			}
		}

		for ( i = 1; i < row-1; i++ )
			for ( j = 1; j < col-1; j++ )
			{
				e = i * col + j;
				if(imlabel[e]==2)
				{
					ee = (i-1) * (col-2) + j - 1;
					im[ee] = 1;
				}
			}

		/*for (j = 0; j < col; j++)  //�߽���0,����Ϊ������
		{
			im[j] = 0;
			im[(row-1)*col+j] = 0;
		}
		for(i = 0; i < row; i++)
		{
			im[i*col] = 0;
			im[i*col+col-1] = 0;
		}*/

		DestroyStack(S);
		delete []imlabel;
		delete []fill;
		delete []imcopy;
		return 1;
}


DLL_API	int bwFillHole1(unsigned char *im,
					   int row,
					   int col,
					   int AreaThreshold);
DLL_API	int bwFillHole1(unsigned char *im,
					   int row,
					   int col,
					   int AreaThreshold)
{
	int size = row * col;
	int i,j,k,m,n,e,ee,count;
	//int number = 0;
	Stack S;
	if(!InitStack(S)) return 0;
	if (AreaThreshold==0) return 1;
	unsigned char *imlabel = new unsigned char[size];

	for (j = 0; j < col; j++)  //�߽���1,����Ϊǰ����
	{
		im[j] = 1;
		im[(row-1)*col+j] = 1;
	}
	for(i = 0; i < row; i++)
	{
		im[i*col] = 1;
		im[i*col+col-1] = 1;
	}

	int *fill = new int[AreaThreshold];//����Ҫ���������ֵ

	for(i = 0; i < size; i++)
		imlabel[i] = 0;
	for(i = 0; i < AreaThreshold; i++)
		fill[i] = 0;
	for(i = 1; i < row-1; i++)
		for(j = 1; j < col-1; j++)
		{
			e = i * col + j;
			if( im[e]==0 && imlabel[e]==0 )
			{
				fill[0] = e;
				count = 0;
				//number++;
				imlabel[e] = 1;
				if(!Push(S,e))
					return 0;
				while(!StackEmpty(S))
				{
					e = Pop(S);
					for(m = -1; m <= 1; m++)
						for(n = -1; n <= 1;n++)
						{
							if ( m*n==0 )
							{
								ee = e + m * col + n;
								if( im[ee]==0 && imlabel[ee]==0 )
								{
									if(!Push(S,ee))
										return 0;
									imlabel[ee] = 1;
									count++;
									if(count < AreaThreshold)
										fill[count] = ee;
								}
							}

						}
				}
				if( count < AreaThreshold)
				{
					for(k = 0; k <= count; k++)
						imlabel[fill[k]] = 2;
				}
			}
		}
		for(i = 0; i < size; i++)
		{
			if(imlabel[i]==2)
				im[i] = 1;
			//im[i] = imlabel[i];
		}

		for (j = 0; j < col; j++)  //�߽���0,����Ϊ������
		{
			im[j] = 0;
			im[(row-1)*col+j] = 0;
		}
		for(i = 0; i < row; i++)
		{
			im[i*col] = 0;
			im[i*col+col-1] = 0;
		}

		DestroyStack(S);
		delete []imlabel;
		delete []fill;
		return 1;
}






DLL_API	int bwremove_restriction(unsigned char *im,
					 int row,
					 int col,
					 int AreaThreshold);
DLL_API	int bwremove_restriction(unsigned char *im,
					 int row,
					 int col,
					 int AreaThreshold)
{
	row += 2;
	col += 2;
	int size = row * col; //���ӱ߽�
	int i,j,k,m,n,e,ee,count;
	//int number = 0;
	Stack S;
	if(!InitStack(S)) return 0;
	if (AreaThreshold==0) return 1;
	unsigned char *imlabel = new unsigned char[size];
	unsigned char *imcopy = new unsigned char[size];
	int *remove = new int[AreaThreshold];

	for(i = 0; i < size; i++)
	{
		imlabel[i] = 0;
		imcopy[i] = 0;
	}

	for(i = 0; i < AreaThreshold; i++)
		remove[i] = 0;

	for ( i = 1; i < row-1; i++ )
		for ( j = 1; j < col-1; j++ )
		{
			e = i * col + j;
			ee = (i-1) * (col-2) + j - 1;
			imcopy[e] = im[ee];
		}

		for(i = 1; i < row-2300; i++)
			for(j = 1; j < col-1; j++)
			{
				e = i * col + j;
				if( imcopy[e]!=0 && imlabel[e]==0 )
				{
					remove[0] = e;
					count = 0;
					//number++;
					imlabel[e] = 1;
					if(!Push(S,e))
						return 0;
					while(!StackEmpty(S))
					{
						e = Pop(S);
						for(m = -1; m <= 1; m++)
							for(n = -1; n <= 1;n++)
							{
								ee = e + m * col + n;
								if( imcopy[ee]!=0 && imlabel[ee]==0 )
								{
									if(!Push(S,ee))
										return 0;
									imlabel[ee] = 1;
									count++;
									if(count < AreaThreshold)
										remove[count] = ee;
								}
							}
					}
					if( count < AreaThreshold)
					{
						for(k = 0; k <= count; k++)
							imlabel[remove[k]] = 2;
					}
				}
			}

			for ( i = 1; i < row-1; i++ )
				for ( j = 1; j < col-1; j++ )
				{
					e = i * col + j;
					if(imlabel[e]==2)
					{
						ee = (i-1) * (col-2) + j - 1;
						im[ee] = 0;
					}
				}

				DestroyStack(S);
				delete []imlabel;
				delete []remove;
				delete []imcopy;
				return 1;
}
