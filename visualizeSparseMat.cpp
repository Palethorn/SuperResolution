#include "header.h"

Mat visualizeSparseMat(SparseMat& smat, Size out_imsize)
{
    Mat data = Mat::zeros(out_imsize,CV_8U);
    double inv_size0 = 1.0/smat.size(0);
    double inv_size1 = 1.0/smat.size(1);

    SparseMatIterator it = smat.begin(),it_end = smat.end();
    for(;it!=it_end;++it)
    {
        int j = (int)(((double)it.node()->idx[0]*inv_size0*out_imsize.height));
        int i = (int)(((double)it.node()->idx[1]*inv_size1*out_imsize.width));
        data.at<uchar>(j,i)=255;
    }

    Mat zeromat = Mat::zeros(out_imsize,CV_8U);
    vector<Mat> image;
    image.push_back(zeromat);
    image.push_back(data);
    image.push_back(zeromat);

    Mat ret;
    cv::merge(image,ret);

    cout<<"number of non zero elements: "<<smat.nzcount()<<endl;
    return ret;
}