//#include <core.hpp>
#include <highgui.h>
#include <stdio.h>
#include <cv.h>
#ifdef _OPENMP
#include <omp.h>
#endif
using namespace cv;
#include <iostream>
using namespace std;

void sum_float_OMP(Mat src[], Mat& dest, int numofview, float beta);

//sparse matrix and matrix function
void mulSparseMat32f(SparseMat& smat, Mat& src, Mat& dest, bool isTranspose = false);
Mat visualizeSparseMat(SparseMat& smat, Size out_imsize);
void subtract_sign(Mat& src1, Mat&src2, Mat& dest);//dest = sign(src1-src2);

//image processing
double getPSNR(Mat& src1, Mat& src2, int bb);
void addspikenoise(Mat& src, Mat& dest, int val);
void addgaussnoise(Mat& src, Mat& dest, double sigma);

//for super resolution
SparseMat createDownsampledMotionandBlurCCDSparseMat32f(Mat& src, int amp, Point2d move);
SparseMat createDegradedImageandSparseMat32F(Mat& src, Mat& dest, Point2d move, int amp);
void btvregularization(Mat& srcVec, Size kernel, float alpha, Mat& dstVec, Size size);

#define sign_float(a,b) (a>b)?1.0f:(a<b)?-1.0f:0.0f

enum
{
    SR_DATA_L1=0,
    SR_DATA_L2
};

//Bilateral Total Variation based Super resolution with
void superresolutionSparseMat32f(Mat src[], Mat& dest, SparseMat DHF[], const int numofview,int iteration, float beta, float lambda, float alpha, Size reg_window,int method, Mat& ideal);
