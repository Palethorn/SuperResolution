#include "header.h"

SparseMat createDegradedImageandSparseMat32F(Mat& src, Mat& dest, Point2d move, int amp)
{
    SparseMat DHF=createDownsampledMotionandBlurCCDSparseMat32f(src,amp,move);

    int matsize =src.cols*src.rows ;
    int matsize2 =src.cols*src.rows/(amp*amp);

    Mat svec;
    src.reshape(3,matsize).convertTo(svec,CV_32FC3);
    Mat dvec(matsize2,1,CV_32FC3);

    mulSparseMat32f(DHF,svec,dvec);

    imshow("smat",visualizeSparseMat(DHF,Size(512,512/amp/amp)));waitKey(30);
    //re-convert  1D vecor structure to Mat image structure
    dvec.reshape(3,dest.rows).convertTo(dest,CV_8UC3);

    return DHF;
}