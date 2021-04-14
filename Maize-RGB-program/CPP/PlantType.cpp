#define DLL_API extern "C" _declspec(dllexport)
#include<stdio.h>
#include <math.h>
#include "stdlib.h"

#define STACK_INIT_SIZE 1000//存储空间初始分配量
#define STACKINCREMENT 100//存储空间分配增量

typedef struct
{
	int *base;//栈底指针
	int *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位
}Stack;

//构造一个空栈S
bool InitStack(Stack &S)
{
	S.base =(int *) malloc(STACK_INIT_SIZE * sizeof(int));
	if(!S.base)
		return false;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return true;
}

//入栈，插入元素e为新的栈顶元素
bool Push(Stack &S, int e)
{
	if(S.top - S.base >= S.stacksize)//栈满，追加存储空间
	{
		S.base = (int*) realloc (S.base, (S.stacksize + STACKINCREMENT) * sizeof(int));
		if(!S.base) return false;//存储分配失败
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return true;
}

//出栈，若栈不空，删除S的栈顶元素，用e返回其值
int Pop(Stack &S)
{
	if(S.top == S.base) return false;
	//*e = * --S.top;
	return * --S.top;
}
//若栈S为空栈，返回1，否则返回0
bool StackEmpty(Stack S)
{
	return S.top == S.base;
}

//销毁栈S
void DestroyStack(Stack &S)
{
	delete []S.base;
	S.base = NULL;
	S.top = NULL;
}






/****************************/

/*******越界判断******/

bool IsOutBoud(int R_id, int C_id, int row, int col)
{
	if ( (R_id < 0) || (R_id >= row) || (C_id < 0) || (C_id >= col) )
		return true;
	else
		return false;
}

/****************************/

/*****叶密度，R1-R6********/

DLL_API void CalLeafDens(unsigned char *bw,
						 int row,
						 int col,
						 int GridSize,
						 float *dens);
DLL_API void CalLeafDens(unsigned char *bw,
						 int row,
						 int col,
						 int GridSize,
						 float *dens)
{
	int i, j, m, n;
	int R_id, C_id, index, count;
	if ( GridSize <= 0 )
		return;
	int nRow = ( row + GridSize - 1) / GridSize;
	int nCol = ( col + GridSize - 1) / GridSize;
	float GridNum = GridSize * GridSize;
	float dens_temp;

	for ( i = 0; i < nRow; i++ )
		for ( j = 0; j < nCol; j++ )
		{
			R_id = i * GridSize;
			C_id = j * GridSize;
			count = 0;
			for ( m = 0; m < GridSize; m++ )
				for ( n = 0; n < GridSize; n++ )
				{
					if ( !IsOutBoud(R_id+m, C_id+n, row, col) )
					{
						index = ( R_id + m ) * col + C_id + n;
						if ( bw[index] != 0 )
							count++;
					}

				}
				dens_temp = count / GridNum;
				for ( m = 0; m < GridSize; m++ )
					for ( n = 0; n < GridSize; n++ )
					{
						if ( IsOutBoud(R_id+m, C_id+n, row, col) == false )
						{
							index = ( R_id + m ) * col + C_id + n;
							if ( bw[index] != 0 )
								dens[index] += dens_temp;
						}
					}
		}

}


DLL_API void DensClassify(float *dens,
						  int row,
						  int col,
						  float *ratio);
DLL_API void DensClassify(float *dens,
						  int row,
						  int col,
						  float *ratio)
{
	int i, size, count;
	size = row * col;
	count = 0;
	int level;
	for ( i = 0; i < size; i++ )
	{
		if ( dens[i] > float(0) )
		{
			count++;
			level = dens[i] / 0.1;
			switch( level )
			{
			case 0:
				ratio[0]++;
				break;
			case 1:
				ratio[1]++;
				break;
			case 2:
			case 3:
				ratio[2]++;
				break;
			case 4:
			case 5:
				ratio[3]++;
				break;
			case 6:
			case 7:
				ratio[4]++;
				break;
			case 8:
			case 9:
			case 10:
				ratio[5]++;
				break;
			default:
				break;
			}

		}

	}
	for ( i = 0; i < 6; i++ )
		ratio[i] /= count; 

}
