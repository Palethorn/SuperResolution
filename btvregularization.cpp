#include "header.h"

void btvregularization(Mat& srcVec, Size kernel, float alpha, Mat& dstVec, Size size)
{
    Mat src;
    srcVec.reshape(3,size.height).convertTo(src,CV_32FC3);
    Mat dest(size.height,size.width,CV_32FC3);

    const int kw = (kernel.width-1)/2;
    const int kh = (kernel.height-1)/2;

    float* weight = new float[kernel.width*kernel.height];
    for(int m=0,count=0;m<=kh;m++)
    {
        for(int l=kw;l+m>=0;l--)
        {
            weight[count]=pow(alpha,abs(m)+abs(l));
            count++;
        }
    }
//a part of under term of Eq (22) lambda*\sum\sum ...
#pragma omp parallel for
    for(int j=kh;j<src.rows-kh;j++)
    {
        float* s = src.ptr<float>(j);
        float* d = dest.ptr<float>(j);
        for(int i=kw;i<src.cols-kw;i++)
        {
            float r=0.0;
            float g=0.0;
            float b=0.0;

            const float sr=s[3*(i)+0];
            const float sg=s[3*(i)+1];
            const float sb=s[3*(i)+2];
            for(int m=0,count=0;m<=kh;m++)
            {
                float* s2 = src.ptr<float>(j-m);
                float* ss = src.ptr<float>(j+m);
                for(int l=kw;l+m>=0;l--)
                {
                    r+=weight[count]*(sign_float(sr,ss[3*(i+l)+0]) -sign_float(s2[3*(i-l)+0],sr));
                    g+=weight[count]*(sign_float(sg,ss[3*(i+l)+1]) -sign_float(s2[3*(i-l)+1],sg));
                    b+=weight[count]*(sign_float(sb,ss[3*(i+l)+2]) -sign_float(s2[3*(i-l)+2],sb));
                    count++;
                }
            }
            d[3*i+0]=r;
            d[3*i+1]=g;
            d[3*i+2]=b;
        }
    }
    dest.reshape(3,size.height*size.width).convertTo(dstVec,CV_32FC3);
    delete[] weight;
}