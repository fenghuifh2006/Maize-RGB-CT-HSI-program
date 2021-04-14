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
	int s;//��/�к��Ӹ���
	int x,y;
	int px,py;

	double NP_sum=0;//�ܵ���
	for (int i=0;i<M*M;i++)
		NP_sum=+BW[i];


	s=ceil(double(M)/double(r));
	for (i=0;i<s*s;i++)
	{
		NB[i]=0;
	}

	//����ÿ�������������ǰ������
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
	int sr,sc;//��/�к��Ӹ���
	int x,y;
	int px,py;

	double NP_sum=0;//�ܵ���
	for (int i=0;i<row*col;i++)
		NP_sum=+BW[i];


	sr=ceil(double(row)/double(r));
	sc=ceil(double(col)/double(r));
	for (i=0;i<sr*sc;i++)
	{
		NB[i]=0;
	}

	//����ÿ�������������ǰ������
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
BoxCounting ���ڼ�������ͼ��ļƺ�ά��

*BW        --�����ֵͼ��
M          --ͼ���С��M*M��
rmin       --���ӱ߳���Сֵ
rmax       --���ӱ߳����ֵ
*logr      --�߳�����
*logNr     --Nr���� 
*/
DLL_API float BoxCounting(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *logNr);
DLL_API float BoxCounting(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *logNr)
{
	int i,j,r;
	int s;//��/�к��Ӹ���
	int x,y;
	int px,py;
	int PN;//������

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
BoxCounting ���ڼ�������ͼ��ļƺ�ά��

*BW        --�����ֵͼ��
row         --ͼ����
col          --ͼ����
rmin       --���ӱ߳���Сֵ
rmax       --���ӱ߳����ֵ
*logr      --�߳�����
*logNr     --Nr���� 
*/
DLL_API float BoxCounting2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *logNr);
DLL_API float BoxCounting2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *logNr)
{
	int i,j,r;
	int sr,sc;//��,�к��Ӹ���
	int x,y;
	int px,py;
	int PN;//������

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
InformationDimension ���ڼ�������ͼ�����Ϣά��

*BW        --�����ֵͼ��
M          --ͼ���С��M*M��
rmin       --���ӱ߳���Сֵ
rmax       --���ӱ߳����ֵ
*/
DLL_API float InformationDimension(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *Ir);
DLL_API float InformationDimension(unsigned char *BW,int M,int rmin,int rmax,double *logr,double *Ir)
{
	int i,r;
	int s;//��/�к��Ӹ���
	int PN;//������
	PN=rmax-rmin+1;

	double NP_sum=0;//�ܵ���
	for (int i=0;i<M*M;i++)
		NP_sum=NP_sum+BW[i];

	for (r=rmin;r<=rmax;r++)
	{
		s=ceil(double(M)/double(r));
		double *NB=new double[s*s];//ÿ�������е�ǰ������
		double p;
		for (i=0;i<s*s;i++)
		{
			NB[i]=0;
		}

		/*
		//����ÿ�������������ǰ������
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
					p=NB[i]/(NP_sum);//ǰ�������뵱ǰ���ӵĸ���
					Ir[r-rmin]+=p*log(1/p);
				}
			}
			delete[]NB;
			logr[r-rmin]=log(double(r));
	}


	return 1;

}

/*
InformationDimension2 ���ڼ�������ͼ�����Ϣά��

*BW        --�����ֵͼ��
row        --ͼ����
col        --ͼ����
rmin       --���ӱ߳���Сֵ
rmax       --���ӱ߳����ֵ
*/
DLL_API float InformationDimension2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *Ir);
DLL_API float InformationDimension2(unsigned char *BW,int row,int col,int rmin,int rmax,double *logr,double *Ir)
{
	int i,r;
	int sr,sc;//��/�к��Ӹ���
	int PN;//������
	PN=rmax-rmin+1;

	double NP_sum=0;//�ܵ���
	for (int i=0;i<row*col;i++)
		NP_sum=NP_sum+BW[i];

	for (r=rmin;r<=rmax;r++)
	{
		sr=ceil(double(row)/double(r));
		sc=ceil(double(col)/double(r));
		double *NB=new double[sr*sc];//ÿ�������е�ǰ������
		double p;
		for (i=0;i<sr*sc;i++)
		{
			NB[i]=0;
		}

		/*
		//����ÿ�������������ǰ������
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
				p=NB[i]/(NP_sum);//ǰ�������뵱ǰ���ӵĸ���
				Ir[r-rmin]+=p*log(1/p);
			}
		}
		delete[]NB;
		logr[r-rmin]=log(double(r));
	}


	return 1;

}
