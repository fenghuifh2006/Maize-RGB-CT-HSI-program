#include<windows.h>
#include<stdio.h>
#include <math.h>
#include <string.h>

/*提取图像的直方图特征，包括背景像素
*Im        --输入的灰度图像
row        --行
col        --列
G          --灰度级
*HistStat  --直方图灰度级,为1*6的一维数组，按顺序存放均值、标准差、平滑度，三阶矩，一致性，熵
*/

extern "C" _declspec(dllexport) void HistProperty(unsigned char *Im,int row,int col,int G,double *HistStat);
extern "C" _declspec(dllexport) void HistProperty(unsigned char *Im,int row,int col,int G,double *HistStat)
{
	int i;
	double m=0,sigma=0,R=0,mu3=0,U=0,entropy=0;//均值、标准差、平滑度，三阶矩，一致性，熵
	int level;
	int size=row*col;

	double *hist= new double [G];
	double *p=new double [G];
	
	for (i=0;i<G;i++)
	{
		hist[i]=0;
		p[i]=0;
	}

	//计算直方图
	for (i=0;i<size;i++)
	{
		level=Im[i];
			hist[level]++;
	}

	for (i=0;i<G;i++)
	{
		p[i]=hist[i]/size;
		m+=i*p[i];
	}

	for (i=0;i<G;i++)
	{
		sigma+=(i-m)*(i-m)*p[i];
		mu3+=(i-m)*(i-m)*(i-m)*p[i];
		U+=p[i]*p[i];
		if (p[i]!=0)
		entropy+=p[i]*log(p[i])/log(double(2));
	}
	sigma=sqrt(sigma);
	R=1-1/(1+sigma);

	HistStat[0]=m;
	HistStat[1]=sigma;
	HistStat[2]=R;
	HistStat[3]=mu3;
	HistStat[4]=U;
	HistStat[5]=-entropy;

	delete [] hist;
	delete []p;
}

/*提取图像前景部分的直方图特征
*Im        --输入的灰度图像
row        --行
col        --列
G          --灰度级
*HistStat  --直方图灰度级,为1*6的一维数组，按顺序存放均值、标准差、平滑度，三阶矩，一致性，熵
*/

extern "C" _declspec(dllexport) void HistProperty2(unsigned char *Im,int row,int col,int G,double *HistStat);
extern "C" _declspec(dllexport) void HistProperty2(unsigned char *Im,int row,int col,int G,double *HistStat)
{
	int i;
	double m=0,sigma=0,R=0,mu3=0,U=0,entropy=0;//均值、标准差、平滑度，三阶矩，一致性，熵
	int level;
	int size=row*col;
	int area=0;

	double *hist= new double [G-1];
	double *p=new double [G-1];

	for (i=0;i<G;i++)
	{
		hist[i]=0;
		p[i]=0;
	}

	//计算直方图
	for (i=0;i<size;i++)
	{
		level=Im[i];
		if (level!=0)
		{
			hist[level-1]++;
			area++;
		}
	}

	for (i=0;i<G-1;i++)
	{
		p[i]=hist[i]/area;
		m+=(i+1)*p[i];
	}

	for (i=0;i<G-1;i++)
	{
		sigma+=(i+1-m)*(i+1-m)*p[i];
		mu3+=(i+1-m)*(i+1-m)*(i+1-m)*p[i];
		U+=p[i]*p[i];
		if (p[i]!=0)
			entropy+=p[i]*log(p[i])/log(double(2));
	}
	sigma=sqrt(sigma);
	R=1-1/(1+sigma*sigma);

	HistStat[0]=m;
	HistStat[1]=sigma;
	HistStat[2]=R;
	HistStat[3]=mu3;
	HistStat[4]=U;
	HistStat[5]=-entropy;

	delete [] hist;
	delete []p;
}