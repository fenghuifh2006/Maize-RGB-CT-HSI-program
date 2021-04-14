#include<windows.h>
#include<stdio.h>
#include <math.h>
#include <string.h>

/*��ȡͼ���ֱ��ͼ������������������
*Im        --����ĻҶ�ͼ��
row        --��
col        --��
G          --�Ҷȼ�
*HistStat  --ֱ��ͼ�Ҷȼ�,Ϊ1*6��һά���飬��˳���ž�ֵ����׼�ƽ���ȣ����׾أ�һ���ԣ���
*/

extern "C" _declspec(dllexport) void HistProperty(unsigned char *Im,int row,int col,int G,double *HistStat);
extern "C" _declspec(dllexport) void HistProperty(unsigned char *Im,int row,int col,int G,double *HistStat)
{
	int i;
	double m=0,sigma=0,R=0,mu3=0,U=0,entropy=0;//��ֵ����׼�ƽ���ȣ����׾أ�һ���ԣ���
	int level;
	int size=row*col;

	double *hist= new double [G];
	double *p=new double [G];
	
	for (i=0;i<G;i++)
	{
		hist[i]=0;
		p[i]=0;
	}

	//����ֱ��ͼ
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

/*��ȡͼ��ǰ�����ֵ�ֱ��ͼ����
*Im        --����ĻҶ�ͼ��
row        --��
col        --��
G          --�Ҷȼ�
*HistStat  --ֱ��ͼ�Ҷȼ�,Ϊ1*6��һά���飬��˳���ž�ֵ����׼�ƽ���ȣ����׾أ�һ���ԣ���
*/

extern "C" _declspec(dllexport) void HistProperty2(unsigned char *Im,int row,int col,int G,double *HistStat);
extern "C" _declspec(dllexport) void HistProperty2(unsigned char *Im,int row,int col,int G,double *HistStat)
{
	int i;
	double m=0,sigma=0,R=0,mu3=0,U=0,entropy=0;//��ֵ����׼�ƽ���ȣ����׾أ�һ���ԣ���
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

	//����ֱ��ͼ
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