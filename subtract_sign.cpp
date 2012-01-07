#include "header.h"

void subtract_sign(Mat& src1, Mat& src2, Mat& dest)
{
    for(int j=0;j<src1.rows;j++)
    {
        float* s1 = src1.ptr<float>(j);
        float* s2 = src2.ptr<float>(j);
        float* d = dest.ptr<float>(j);
        for(int i=0;i<src1.cols;i++)
        {
            d[3*i]=sign_float(s1[3*i],s2[3*i]);
            d[3*i+1]=sign_float(s1[3*i+1],s2[3*i+1]);
            d[3*i+2]=sign_float(s1[3*i+2],s2[3*i+2]);
        }
    }
}
