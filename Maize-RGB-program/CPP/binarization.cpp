#define DLL_API extern "C" _declspec(dllexport)
#include<windows.h>
#include<stdio.h>
#include <math.h>



DLL_API void TOPim2bw2(unsigned char *R,
							 unsigned char *G,
							 unsigned char *B,
							 int row,
							 int col,
							 float EGthreshold,
							 float ERthreshold,
							 unsigned char *bw);
DLL_API void TOPim2bw2(unsigned char *R,
							 unsigned char *G,
							 unsigned char *B,
							 int row,
							 int col,	
							 float EGthreshold,
							 float ERthreshold,
							 unsigned char *bw)
{
	int i, size;

	size = row * col;
	float *r = new float[size]; float *g = new float[size]; float *b = new float[size];
	float *EG = new float[size]; float *ER = new float[size];
	float RGB, R_G,R_B,G_B;


		for ( i = 0; i < size; i++ )
		{
			r[i] = .0; g[i] = .0; r[i] = .0; EG[i] = .0; ER[i] = .0;
			RGB = R[i] + G[i] + B[i];
			G_B = float(G[i] + 1)/float(B[i] + 1);
			R_B = float(R[i] + 1)/float(B[i] + 1); 
			R_G =  float(R[i] + 1)/float(G[i] + 1);


			if ( RGB > 30)
			{
				r[i] = R[i] / RGB; g[i] = G[i] / RGB; b[i] = B[i] / RGB;
				EG[i] = 2 * g[i] - r[i] - b[i];
				EG[i] = EG[i] > EGthreshold ? 1 : 0;			
				ER[i] = 0;
			} else EG[i] = ER[i] = 0;
		}




	for(i = 0; i< size; i++)
		bw[i] = EG[i] - ER[i] > 0 ? 1 : 0;

	/*for (j = 4500; j< 5500; j++)
	{
	for (k= 0; k<400; k++)
	{
	bw[j*4000 + k] = 0;
	}
	}*/



	delete []r;
	delete []g;
	delete []b;
	delete []EG;
	delete []ER;
}