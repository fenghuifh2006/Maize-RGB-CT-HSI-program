// Thinning.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Thinning.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "memory.h"
static int erasetable[256] =
{
  0,0,1,1,0,0,1,1,        1,1,0,1,1,1,0,1,
  1,1,0,0,1,1,1,1,        0,0,0,0,0,0,0,1,
  0,0,1,1,0,0,1,1,        1,1,0,1,1,1,0,1,
  1,1,0,0,1,1,1,1,        0,0,0,0,0,0,0,1,
  1,1,0,0,1,1,0,0,        0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,
  1,1,0,0,1,1,0,0,        1,1,0,1,1,1,0,1,
  0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,
  0,0,1,1,0,0,1,1,        1,1,0,1,1,1,0,1,
  1,1,0,0,1,1,1,1,        0,0,0,0,0,0,0,1,
  0,0,1,1,0,0,1,1,        1,1,0,1,1,1,0,1,
  1,1,0,0,1,1,1,1,        0,0,0,0,0,0,0,0,
  1,1,0,0,1,1,0,0,        0,0,0,0,0,0,0,0,
  1,1,0,0,1,1,1,1,        0,0,0,0,0,0,0,0,
  1,1,0,0,1,1,0,0,        1,1,0,1,1,1,0,0,
  1,1,0,0,1,1,1,0,        1,1,0,0,1,0,0,0
};
/*对图像边缘进行扩展*/
bool Expand(unsigned char *Array,unsigned char *pi,unsigned int _width, unsigned int _height)
{
	unsigned int x,y;
	for(y=0; y<_height;y++)//注意防止越界
	{
		for(x=0;x<_width;x++)
		{
			if(x==0||x==_width-1||y==0||y==_height-1)
			{
				pi[y * _width + x] = 0;
			}
			else
			{
				pi[y * _width + x] = Array[(y-1) * (_width-2) + x-1];
			}
		}
	}
	return true;
}
/*对图像边缘进行恢复*/
bool Erode(unsigned char *Array,unsigned char *pi,unsigned int _width, unsigned int _height)
{
	unsigned int x,y;
	for(y=1; y<_height-1;y++)//注意防止越界
	{
		for(x=1;x<_width-1;x++)
		{
			
			Array[(y-1) * (_width-2) + x-1] = pi[y * _width + x];
		}
	}
	return true;
}
int Thinning(unsigned char *Array, unsigned int width, unsigned int height)
{
    bool Finished;
    int nw, n, ne, w, e, sw, s, se;
    unsigned int x,y;
    int num;
	unsigned int _width = width +2;
	unsigned int _height = height +2;
    int BufSize = _width*_height;
	unsigned char *pi;
	unsigned char *pi_new;
    Finished = false;//结束标志置成假；
    pi_new = new unsigned char[BufSize];
	pi = new unsigned char[BufSize];
	if(!Expand(Array,pi,_width,_height)) 
		return 0;
    while (!Finished)
    {
        Finished = true;
        //先进行水平方向细化；
        for (y = 1; y < _height - 1; y++)//注意防止越界；
        {
            x = 1;
            while (x < _width - 1)
            {
                if (pi[BufSize - y * _width - _width + x] == 0)
                {
                    w = pi[BufSize - y * _width - _width + x - 1];//左邻点
                    e = pi[BufSize - y * _width - _width + x + 1];//右邻点
                    if((w==255)||(e==255))//如果左右两个邻居中至少有一个是白点才处理
                    {
                        nw = pi[BufSize - y * _width + x - 1];
                        n  = pi[BufSize - y * _width + x ];
                        ne = pi[BufSize - y * _width + x+1];
                        sw = pi[BufSize - y * _width - 2*_width + x - 1];
                        s  = pi[BufSize - y * _width - 2*_width + x];
                        se = pi[BufSize - y * _width - 2*_width + x+1];
                        num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
                        if(erasetable[num]==1)//经查表可以删除
                        {
                          pi[BufSize - y * _width - _width + x]=(unsigned char)255;
                          pi_new[BufSize - y * _width - _width + x] = (unsigned char)255;
                          Finished= false;//有改动，结束标志置成假；
                          x++;//水平方向跳过一个像素；
                        }
                    }
                }
                x++;//扫描下一个像素；
            }
        }
        //再进行垂直方向的细化
        for(x=1;x< _width - 1;x++)
        {
            y=1;
            while(y< _height - 1)
            {
                if (pi[BufSize - y * _width - _width + x] == 0)
                {
                    n  = pi[BufSize - y * _width + x ];
                    s  = pi[BufSize - y * _width - 2*_width + x];
                    if ((n == 255) || (s == 255))
                    {
                        w = pi[BufSize - y * _width - _width + x - 1];//左邻点
                        e = pi[BufSize - y * _width - _width + x + 1];//右邻点
                        nw = pi[BufSize - y * _width + x - 1];
                        ne = pi[BufSize - y * _width + x + 1];
                        sw = pi[BufSize - y * _width - 2 * _width + x - 1];
                        se = pi[BufSize - y * _width - 2 * _width + x + 1];
                        num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 + sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;
                        if (erasetable[num] == 1)//经查表可以删除
                        {
                            pi[BufSize - y * _width - _width + x] = (unsigned char)255;
                            pi_new[BufSize - y * _width - _width + x] = (unsigned char)255;
                            Finished = false;//有改动，结束标志置成假；
                            y++;//水平方向跳过一个像素；
                        }
                    }

                }
                y++;
            }
        }

    }
    for (y = 1; y < _height - 1; y++)//注意防止越界；
        for (x = 1; x < _width - 1; x++)
        {
            pi[y * _width + x] = (unsigned char)(255 - pi[y * _width + x]);
        }
	Erode(Array,pi,_width,_height);
	delete pi_new;//释放占用的相应内存堆区
	delete pi;//释放占用的相应内存堆区
	return 1;
}


