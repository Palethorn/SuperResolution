#include "header.h"

SparseMat createDownsampledMotionandBlurCCDSparseMat32f(Mat& src, int amp, Point2d move)
{
    //(1)'
    //D down sampling matrix
    //H blur matrix, in this case, we use only ccd sampling blur.
    //F motion matrix, in this case, threr is only global shift motion.

    float div = 1.0f/((float)(amp*amp));
    int x1 = (int)(move.x+1);
    int x0 = (int)(move.x);
    float a1 = (float)(move.x-x0);
    float a0 = (float)(1.0-a1);

    int y1 = (int)(move.y+1);
    int y0 = (int)(move.y);
    float b1 = (float)(move.y-y0);
    float b0 = (float)(1.0-b1);

    int bsx =x1;
    int bsy =y1;

    int matsize =src.cols*src.rows ;
    int matsize2 =src.cols*src.rows/(amp*amp);
    int size2[2]={matsize,matsize2};
    SparseMat DHF(2,size2,CV_32FC1);

    const int step = src.cols/amp;
    for(int j=0;j<src.rows;j+=amp)
    {
        for(int i=0;i<src.cols;i+=amp)
        {
            int y = src.cols*j+i;
            int s = step*j/amp+i/amp;

            if(i>=bsx &&i<src.cols-bsx-amp &&j>=bsy &&j<src.rows-bsy-amp)
            {
                for(int l=0;l<amp;l++)
                {
                    for(int k=0;k<amp;k++)
                    {
                        DHF.ref<float>(y+src.cols*(y0+l)+x0+k,s)+=a0*b0*div;
                        DHF.ref<float>(y+src.cols*(y0+l)+x1+k,s)+=a1*b0*div;
                        DHF.ref<float>(y+src.cols*(y1+l)+x0+k,s)+=a0*b1*div;
                        DHF.ref<float>(y+src.cols*(y1+l)+x1+k,s)+=a1*b1*div;
                    }
                }
            }
        }
    }
    return DHF;
}