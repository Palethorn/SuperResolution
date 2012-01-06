#include "header.h"

double getPSNR(Mat& src1, Mat& src2, int bb)
{
    int i,j;
    double sse,mse,psnr;
    sse = 0.0;
  
    Mat s1,s2;
    cvtColor(src1,s1,CV_BGR2GRAY);
    cvtColor(src2,s2,CV_BGR2GRAY);

    int count=0;
    for(j=bb;j<s1.rows-bb;j++)
    {
        uchar* d=s1.ptr(j);
        uchar* s=s2.ptr(j);
  
        for(i=bb;i<s1.cols-bb;i++)
        {
            sse += ((d[i] - s[i])*(d[i] - s[i]));
            count++;
        }
    }
    if(sse == 0.0 || count==0)
    {
        return 0;
    }
	else
    {
        mse =sse /(double)(count);
        psnr = 10.0*log10((255*255)/mse);
        return psnr;
	}
}