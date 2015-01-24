//Function��Ѱ������������CvSeq���÷�
//Source: http://blog.csdn.net/augusdi/article/details/9000276
//Status:����OpenCV1
//Improve:
//Info:[11/15/2014 jmy]


#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>

// #pragma comment(lib, "cv.lib")
// #pragma comment(lib, "cxcore.lib")
// #pragma comment(lib, "highgui.lib")

int contour3( int argc, char** argv )
{
	//����IplImageָ��
	IplImage* pImg = NULL; 
	IplImage* pContourImg = NULL;
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contour = 0;
	int mode = CV_RETR_EXTERNAL;// CV_RETR_EXTERNAL
	unsigned th=0;
	if( argc == 3)
		if(strcmp(argv[2], "all") == 0)
			mode = CV_RETR_CCOMP; //������������� 

	cvNamedWindow("src", 1);
	cvNamedWindow("contour",1);
	
	//�ԻҶ�ͼ�����룬Ȼ��ǿ�ƽ��Ҷ�ͼת��Ϊ��ɫͼ
	if( (pImg = cvLoadImage("samples//200.png", 0)) != 0 )
	{
		cvShowImage( "src", pImg );
		//Ϊ������ʾͼ������ռ䣬3ͨ��ͼ���Ա��ò�ɫ��ʾ
		pContourImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
		//copy source image and convert it to BGR image
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);//ֻ�����ǽ�ÿ��ͨ����������ͬ�ĻҶ�ֵ
		cvCanny(pImg, pImg, 50, 150, 5); //�ڱ�Ե���ͼ��Ļ�������Ѱ������
		int Num=cvFindContours( pImg, storage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE);
		std::cout<<"The number of Contours is:"<<Num<<std::endl;
		for(;contour!=0;contour=contour->h_next)
		{
			printf("***************************************************\n");
			for(int i=0;i<contour->total;i++)
			{
				CvPoint* p=(CvPoint*)cvGetSeqElem(contour,i);//����ȡ�������е�ÿ����
				printf("p->x=%d,p->y=%d\n",p->x,p->y); 
			}
			//���������� 
			cvDrawContours(pContourImg, contour, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0); 
		}
	}
	else
	{
		cvDestroyWindow( "src" );
		cvDestroyWindow( "contour" );
		cvReleaseMemStorage(&storage);
		return -1;
	}


	cvShowImage( "contour", pContourImg );
	cvWaitKey(0);

	cvDestroyWindow( "src" );
	cvDestroyWindow( "contour" );

	cvReleaseImage( &pImg ); 
	cvReleaseImage( &pContourImg ); 
	cvReleaseMemStorage(&storage);
	return 0;
}

//���ܲ�����
//int main()
//{
//	IplImage *src = cvLoadImage("samples\\200.png");
//	IplImage *gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
//	cvCvtColor(src,gray,CV_BGR2GRAY);
//	//cvCanny(gray, gray, 100, 255);
//	cvThreshold(gray, gray, 100, 255, CV_THRESH_BINARY_INV);
//	cvSmooth(gray,gray,2);
//	IplImage*bin = cvCloneImage(gray);
//	
//	CvMemStorage * storage = cvCreateMemStorage(0);
//	CvSeq * contour = 0;
//	//cvFindeContours�������޸�grayͼ��
//	int Num = cvFindContours(bin, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
//
//	for (; contour != 0; contour = contour->h_next){
//		cvDrawContours(src, contour, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 0, 1, 0);
//	}
//	cvNamedWindow("Result", 1);
//	cvShowImage("Result", src);
//	//contour3(0, NULL);
//	cvWaitKey();
//
//	return 0;
//}