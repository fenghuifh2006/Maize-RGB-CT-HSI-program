#define DLL_API extern "C" _declspec(dllexport)
#include<windows.h>
#include<stdio.h>
#include <math.h>
#include <string.h>

bool IsBoundary(int x,int y,int row,int col);
bool IsBoundary(int x,int y,int row,int col)
{
	if(x>0&&x<col&&y>0&&y<row)
		return false;
	else 
		return true;
}

float NBCalculation(unsigned char *BW,int M,int r,double *NB);
float NBCalculation(unsigned char *BW,int M,int r,double *NB)
{
	int i,j;
	int s;//行/列盒子个数
	int x,y;
	int px,py;

	double NP_sum=0;//总点数
	for (int i=0;i<M*M;i++)
		NP_sum=+BW[i];


	s=ceil(double(M)/double(r));
	for (i=0;i<s*s;i++)
	{
		NB[i]=0;
	}

	//计算每个盒子中落入的前景点数
	for (i=0;i<s;i++)
		for (j=0;j<s;j++)
		{
			for (y=0;y<r;y++)
				for (x=0;x<r;x++)
				{
					py=i*r+y;
					px=j*r+x;
					if(!IsBoundary(px,py,M,M))
					{
						if(BW[py*M+px]==1)
						{
							NB[i*s+j]++;
						}
					}						
				}
		}

		return 1;

}


float NBCalculation2(unsigned char *BW,int row,int col,int r,double *NB);
float NBCalculation2(unsigned char *BW,int row,int col,int r,double *NB)
{
	int i,j;
	int sr,sc;//行/列盒子个数
	int x,y;
	int px,py;

	double NP_sum=0;//总点数
	for (int i=0;i<row*col;i++)
		NP_sum=+BW[i];


	sr=ceil(double(row)/double(r));
	sc=ceil(double(col)/double(r));
	for (i=0;i<sr*sc;i++)
	{
		NB[i]=0;
	}

	//计算每个盒子中落入的前景点数
	for (i=0;i<sr;i++)
		for (j=0;j<sc;j++)
		{
			for (y=0;y<r;y++)
				for (x=0;x<r;x++)
				{
					py=i*r+y;
					px=j*r+x;
					if(!IsBoundary(px,py,row,col))
					{
						if(BW[py*col+px]==1)
						{
							NB[i*sc+j]++;
						}
					}						
				}
		}

		return 1;

}

/*
BoxCounting 用于计算输入图像的计盒维数

*BW        --输入二值图像
M          --图像大小（M*M）
rmin       --盒子边长最小值
rmax       --盒子边长最大值
*logr      --边长对数
*logNr     --Nr对数 
*/
DLL_API float BoxCounting(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *logNr);
DLL_API float BoxCounting(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *logNr)
{
	int i,j,r;
	int s;//行/列盒子个数
	int x,y;
	int px,py;
	int PN;//点数；

	PN=rmax-rmin+1;
	int *Nr=new int[PN];

	for (i=0;i<PN;i++)
		Nr[i]=0;

	for (r=rmin;r<=rmax;r++)
	{
		s=ceil(double(M)/double(r));
		for (i=0;i<s;i++)
			for (j=0;j<s;j++)
			{
				for (y=0;y<r;y++)
					for (x=0;x<r;x++)
					{
						py=i*r+y;
						px=j*r+x;
						if(!IsBoundary(px,py,M,M))
						{
							if(BW[py*M+px]==1)
							{
								Nr[r-rmin]++;
								x=r;
								y=r;
							}
						}						
					}
			}
			
			logr[r-rmin]=log(double(r));
			logNr[r-rmin]=log(double(Nr[r-rmin]));

	}

	
	delete []Nr;

	return 1;

}

/*
BoxCounting 用于计算输入图像的计盒维数

*BW        --输入二值图像
row         --图像行
col          --图像列
rmin       --盒子边长最小值
rmax       --盒子边长最大值
*logr      --边长对数
*logNr     --Nr对数 
*/
DLL_API float BoxCounting2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *logNr);
DLL_API float BoxCounting2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *logNr)
{
	int i,j,r;
	int sr,sc;//行,列盒子个数
	int x,y;
	int px,py;
	int PN;//点数；

	PN=rmax-rmin+1;
	int *Nr=new int[PN];

	for (i=0;i<PN;i++)
		Nr[i]=0;

	for (r=rmin;r<=rmax;r++)
	{
		sr=ceil(double(row)/double(r));
		sc=ceil(double(col)/double(r));
		for (i=0;i<sr;i++)
			for (j=0;j<sc;j++)
			{
				for (y=0;y<r;y++)
					for (x=0;x<r;x++)
					{
						py=i*r+y;
						px=j*r+x;
						if(!IsBoundary(px,py,row,col))
						{
							if(BW[py*col+px]==1)
							{
								Nr[r-rmin]++;
								x=r;
								y=r;
							}
						}						
					}
			}

			logr[r-rmin]=log(double(r));
			logNr[r-rmin]=log(double(Nr[r-rmin]));

	}


	delete []Nr;

	return 1;

}

/*
InformationDimension 用于计算输入图像的信息维数

*BW        --输入二值图像
M          --图像大小（M*M）
rmin       --盒子边长最小值
rmax       --盒子边长最大值
*/
DLL_API float InformationDimension(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *Ir);
DLL_API float InformationDimension(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *Ir)
{
	int i,r;
	int s;//行/列盒子个数
	int PN;//点数；
	PN=rmax-rmin+1;

	double NP_sum=0;//总点数
	for (int i=0;i<M*M;i++)
		NP_sum=NP_sum+BW[i];

	for (r=rmin;r<=rmax;r++)
	{
		s=ceil(double(M)/double(r));
		double *NB=new double[s*s];//每个盒子中的前景点数
		double p;
		for (i=0;i<s*s;i++)
		{
			NB[i]=0;
		}

		/*
		//计算每个盒子中落入的前景点数
		for (i=0;i<s;i++)
			for (j=0;j<s;j++)
			{
				for (y=0;y<r;y++)
					for (x=0;x<r;x++)
					{
						py=i*r+y;
						px=j*r+x;
						if(!IsBoundary(px,py,M,M))
						{
							if(BW[py*M+px]==1)
							{
								NB[i*s+j]++;
							}
						}						
					}
			}
			*/
		NBCalculation(BW,M,r,NB);


			for(i=0;i<s*s;i++)
			{
				if (NB[i]!=0)
				{
					p=NB[i]/(NP_sum);//前景点落入当前盒子的概率
					Ir[r-rmin]+=p*log(1/p);
				}
			}
			delete[]NB;
			logr[r-rmin]=log(double(r));
	}


	return 1;

}

/*
InformationDimension2 用于计算输入图像的信息维数

*BW        --输入二值图像
row        --图像行
col        --图像列
rmin       --盒子边长最小值
rmax       --盒子边长最大值
*/
DLL_API float InformationDimension2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *Ir);
DLL_API float InformationDimension2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *Ir)
{
	int i,r;
	int sr,sc;//行/列盒子个数
	int PN;//点数；
	PN=rmax-rmin+1;

	double NP_sum=0;//总点数
	for (int i=0;i<row*col;i++)
		NP_sum=NP_sum+BW[i];

	for (r=rmin;r<=rmax;r++)
	{
		sr=ceil(double(row)/double(r));
		sc=ceil(double(col)/double(r));
		double *NB=new double[sr*sc];//每个盒子中的前景点数
		double p;
		for (i=0;i<sr*sc;i++)
		{
			NB[i]=0;
		}

		/*
		//计算每个盒子中落入的前景点数
		for (i=0;i<s;i++)
		for (j=0;j<s;j++)
		{
		for (y=0;y<r;y++)
		for (x=0;x<r;x++)
		{
		py=i*r+y;
		px=j*r+x;
		if(!IsBoundary(px,py,M,M))
		{
		if(BW[py*M+px]==1)
		{
		NB[i*s+j]++;
		}
		}						
		}
		}
		*/
		NBCalculation2(BW,row,col,r,NB);


		for(i=0;i<sr*sc;i++)
		{
			if (NB[i]!=0)
			{
				p=NB[i]/(NP_sum);//前景点落入当前盒子的概率
				Ir[r-rmin]+=p*log(1/p);
			}
		}
		delete[]NB;
		logr[r-rmin]=log(double(r));
	}


	return 1;

}
