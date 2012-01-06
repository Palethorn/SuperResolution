#include "header.h"

void addgaussnoise(Mat& src, Mat& dest, double sigma)
{
    Mat noise(src.rows,src.cols,CV_32FC1);
    Mat src_f;
    vector<Mat> images;
    split(src,images);
    for(int c=0;c<src.channels();c++)
    {
        images[c].convertTo(src_f,CV_32FC1);
        randn(noise,Scalar(0.0),Scalar(sigma));
        Mat temp = noise+src_f;
        temp.convertTo(images[c],CV_8UC1);
    }
    merge(images,dest);
}