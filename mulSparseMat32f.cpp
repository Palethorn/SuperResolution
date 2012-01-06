#include "header.h"

void mulSparseMat32f(SparseMat& smat, Mat& src, Mat& dest, bool isTranspose)
{
    dest.setTo(0);
    SparseMatIterator it = smat.begin(),it_end = smat.end();
    if(!isTranspose)
    {
        for(;it!=it_end;++it)
        {
            int i=it.node()->idx[0];
            int j=it.node()->idx[1];
            float* d = dest.ptr<float>(j);
            float* s = src.ptr<float>(i);
            for(int c=0;c<3;c++)
            {
                d[c]+= it.value<float>() * s[c];
            }
        }
    }
    else // for transpose matrix
    {
        for(;it!=it_end;++it)
        {
            int i=it.node()->idx[1];
            int j=it.node()->idx[0];
            float* d = dest.ptr<float>(j);
            float* s = src.ptr<float>(i);
            for(int c=0;c<3;c++)
            {
                d[c]+= it.value<float>() * s[c];
            }
        }
    }
}