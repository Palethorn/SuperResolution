#include "header.h"

void superresolutionSparseMat32f(Mat src[], Mat& dest, SparseMat DHF[], const int numofview,int iteration, float beta, float lambda, float alpha, Size reg_window,int method, Mat& ideal)
{
    //(3) create initial image by simple linear interpolation
    resize(src[0],dest,dest.size());
    cout<<"PSNR"<<getPSNR(dest,ideal,10)<<"dB"<<endl;
    imwrite("linear.png",dest);

    //(4)convert Mat image structure to 1D vecor structure
    Mat dstvec;
    dest.reshape(3,dest.cols*dest.rows).convertTo(dstvec,CV_32FC3);

    Mat* dstvectemp=new Mat[numofview];
    Mat* svec = new Mat[numofview];
    Mat* svec2 = new Mat[numofview];

    for(int n=0;n<numofview;n++)
    {
        src[n].reshape(3,src[0].cols*src[0].rows).convertTo(svec[n],CV_32FC3);
        src[n].reshape(3,src[0].cols*src[0].rows).convertTo(svec2[n],CV_32FC3);

        dstvectemp[n]=dstvec.clone();
    }

    Mat reg_vec=Mat::zeros(dest.rows*dest.cols,1,CV_32FC3);//regularization vector

    //(5)steepest descent method for L1 norm minimization
    for(int i=0;i<iteration;i++)
    {
        cout<<"iteration"<<i<<endl;
        int64 t = getTickCount();
        Mat diff=Mat::zeros(dstvec.size(),CV_32FC3);

        //(5-1)btv
        if(lambda>0.0) btvregularization(dstvec,reg_window,alpha,reg_vec,dest.size());

#pragma omp parallel for
        for(int n=0;n<numofview;n++)
        {
            //degrade current estimated image
            mulSparseMat32f(DHF[n],dstvec,svec2[n]);

            //compere input and degraded image
            Mat temp(src[0].cols*src[0].rows,1, CV_32FC3);
            if(method==SR_DATA_L1)
            {
                subtract_sign(svec2[n], svec[n],temp);
            }
            else
            {
                subtract(svec2[n],svec[n],temp);
                //temp = svec2[n]- svec[n]; //supported in OpenCV2.1
            }

            //blur the subtructed vector with transposed matrix
            mulSparseMat32f(DHF[n],temp,dstvectemp[n],true);
        }
        //creep ideal image, beta is parameter of the creeping speed.
        //add transeposed difference vector. sum_float_OMP is parallelized function of following for loop
        /*for(int n=0;n<numofview;n++)
        {
            addWeighted(dstvec,1.0,dstvectemp[n],-beta,0.0,dstvec);
            //dstvec -= (beta*dstvectemp[n]);//supported in OpenCV2.1
        }*/
        sum_float_OMP(dstvectemp,dstvec,numofview,beta);

        //add smoothness term
        if(lambda>0.0)
        {
            addWeighted(dstvec,1.0,reg_vec,-beta*lambda,0.0,dstvec);
            //dstvec -=lambda*beta*reg_vec;//supported in OpenCV2.1
        }

        //show SR imtermediate process information. these processes does not be required at actural implimentation.
        dstvec.reshape(3,dest.rows).convertTo(dest,CV_8UC3);
        cout<<"PSNR"<<getPSNR(dest,ideal,10)<<"dB"<<endl;

        char name[64];
        sprintf(name,"%03d: %.1f dB",i,getPSNR(dest,ideal,10));
        putText(dest,name,Point(15,50), FONT_HERSHEY_DUPLEX,1.5,CV_RGB(255,255,255),2);

        sprintf(name,"iteration%04d.png",i);
        imshow("SRimage",dest);waitKey(30);
        imwrite(name,dest);
        cout<<"time/iteration"<<(getTickCount()-t)*1000.0/getTickFrequency()<<"ms"<<endl;
    }

    //re-convert  1D vecor structure to Mat image structure
    dstvec.reshape(3,dest.rows).convertTo(dest,CV_8UC3);
    imwrite("sr.png",dest);
}