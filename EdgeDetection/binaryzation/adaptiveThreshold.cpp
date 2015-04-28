/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	AdaptiveFindThreshold.cpp
* Brief: ����Ӧ��ֵ��������������ֵ
* Source:
* Status: 
* Date:	[3/28/2015 jmy]
************************************************************************/

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

/*
 * Function:����Ӧ����Ҷ�ͼ���ֵ���ĸߵ���ֵ
 * Input:
 *		X������ݶ�ͼ
 *		Y������ݶ�ͼ
 * Output:
 *		�ߵ���ֵ����ָ���������ʽ������
 */

void adaptiveThreshold(IplImage *Gradientdx, IplImage *Gradientdy, double *low, double *high)  
{  
	//IplImageת��ΪCvMat
	CvMat* dx=cvCreateMat(Gradientdx->height,Gradientdx->width,CV_32FC1);
	CvMat* dy=cvCreateMat(Gradientdy->height,Gradientdy->width,CV_32FC1);
	cvConvert(Gradientdx,dx);
	cvConvert(Gradientdx,dy);
	
	CvSize size=cvGetSize(Gradientdx);  
	IplImage *imge=0;  
	int i,j;  
	CvHistogram *hist;  
	int hist_size = 255;  
	float range_0[]={0,256};  
	float* ranges[] = { range_0 };  
	double  PercentOfPixelsNotEdges = 0.7;  
	
	imge = cvCreateImage(size, IPL_DEPTH_32F, 1);  
	
	// �����Ե��ǿ��, ������ͼ����  
	float maxv = 0;  
	for(i = 0; i < size.height; i++ )  
	{  
		const short* _dx = (short*)(dx->data.ptr + dx->step*i);  
		const short* _dy = (short*)(dy->data.ptr + dy->step*i);  
		float* _image = (float *)(imge->imageData + imge->widthStep*i);  
		for(j = 0; j < size.width; j++)  
		{  
			_image[j] = (float)(abs(_dx[j]) + abs(_dy[j]));  
			maxv = maxv < _image[j] ? _image[j]: maxv;  
		}  
	}  

	// ����ֱ��ͼ  
	range_0[1] = maxv;  
	hist_size = (int)(hist_size > maxv ? maxv:hist_size);  
	hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);  
	cvCalcHist( &imge, hist, 0, NULL );  
	int total = (int)(size.height * size.width * PercentOfPixelsNotEdges);  
	float sum=0;  
	int icount = hist->mat.dim[0].size;  

	float *h = (float*)cvPtr1D( hist->bins, 0 );  
	for(i = 0; i < icount; i++)  
	{  
		sum += h[i];  
		if( sum > total )  
			break;   
	}  
	
	// ����ߵ�����  
	*high = (i+1) * maxv / hist_size ;  
	*low = *high * 0.4;  
	
	cvReleaseImage( &imge );  
	cvReleaseHist(&hist);  
}  

