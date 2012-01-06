#include "header.h"

void sum_float_OMP(Mat src[], Mat& dest, int numofview, float beta)
{
    for(int n=0;n<numofview;n++)
    {
#pragma omp parallel for
        for(int j=0;j<dest.rows;j++)
        {
            dest.ptr<float>(j)[0]-=beta*src[n].ptr<float>(j)[0];
            dest.ptr<float>(j)[1]-=beta*src[n].ptr<float>(j)[1];
            dest.ptr<float>(j)[2]-=beta*src[n].ptr<float>(j)[2];
        }
    }
}