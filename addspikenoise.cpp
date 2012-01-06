#include "header.h"

void addspikenoise(Mat& src, Mat& dest, int val)
{
    src.copyTo(dest);
#pragma omp parallel for
    for(int j=0;j<src.rows;j++)
    {
        RNG rnd(getTickCount());
        uchar* d = dest.ptr<uchar>(j);
        for(int i=0;i<src.cols;i++)
        {
            if(rnd.uniform(0,val)<1)
            {
                d[3*i]=255;
                d[3*i+1]=255;
                d[3*i+2]=255;
            }
        }
    }
}