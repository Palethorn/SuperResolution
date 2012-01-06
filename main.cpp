//Super resolution with Bilateral Total Variation
//Implimentation of a paper;
//Farsiu, S.,Robinson, D., Elad, M., Milanfar, P."Fast and robust multiframe super resolution," IEEETrans.ImageProcessing 13 (2004)1327?1344.

#include "header.h"

int main(int argc, char** argv)
{
    Mat image = imread("C:\\Users\\David\\Desktop\\lenna1.png", true);//input image
    if(image.empty())
    {
        cout<<"invalid image name"<<endl;
            return -1;
    }
    Mat dest = Mat(image.size(),CV_8UC3);

    const int image_count = 16;//number of input images for super resolution
    const int rfactor = 4;//magnification factor

    Point2d move[image_count];//memory of motion of input images
    SparseMat A[image_count];//degrading matrices
    Mat degimage[image_count];//degraded images

    RNG rnd;//random number generator of image shift/movement

    //(1) generate degraded images and degrading matrices for super resolution
    for(int i=0;i<image_count;i++)
    {
        cout<<i<<endl;
        move[i].x=rnd.uniform(0.0,(double)rfactor);//randam shift for x
        move[i].y=rnd.uniform(0.0,(double)rfactor);//randam shift for y
        if(i==0)// fix first image
        {
            move[i].x=0;
            move[i].y=0;
        }
        Mat imtemp(image.rows/rfactor,image.cols/rfactor,CV_8UC3);
        degimage[i].create(image.rows/rfactor,image.cols/rfactor,CV_8UC3);

        A[i]=createDegradedImageandSparseMat32F(image, imtemp,move[i],rfactor);
        addgaussnoise(imtemp,degimage[i],10.0);//add gaussian noise
        addspikenoise(degimage[i],degimage[i],500);//add spike noise

        imshow("im",degimage[i]);
        char name[32];
        sprintf(name,"input%03d.png",i);
        imwrite(name,degimage[i]);
        waitKey(30);
    }

    //(2) super resolution
    //beta: asymptotic value of steepest descent method
    //lambda: weight parameter to balance data term and smoothness term
    ////alpha: parameter of spacial distribution in btv
    //btv kernel size: kernel size of btv filter
    superresolutionSparseMat32f(degimage,dest,A,image_count,
        180,//number of iteration
        1.3f,//beta
        0.03f,//lambda
        0.7f,//alpha
        Size(7,7),//btv kernel size
        SR_DATA_L1,//L1 norm minimization for data term
        image);//ideal image for evaluation
    return 0;
}