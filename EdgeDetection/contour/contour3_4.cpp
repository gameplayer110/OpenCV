/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	contour3_4.cpp
* Brief: ���������ú������ȼ�ѧϰ,ʹ�õ�OpenCV1.0�ṹ
* Source:
* Status: 
* Date:	[4/3/2015 jmy]
************************************************************************/


//Function��Ѱ������������CvSeq���÷���cvGetSeqElem
//Source: http://blog.csdn.net/augusdi/article/details/9000276
//Status:����OpenCV1
//Improve:
//Info:[11/15/2014 jmy]

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <stdio.h>


int contour3( int argc, char** argv )
{
	//����IplImageָ��
	IplImage* pImg = NULL; 
	IplImage* pContourImg = NULL;
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contour = 0;
	int mode = CV_RETR_EXTERNAL;
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
		pContourImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
		//copy source image and convert it to BGR image
		cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);//ֻ�����ǽ�ÿ��ͨ����������ͬ�ĻҶ�ֵ
		cvCanny(pImg, pImg, 50, 150, 5); //�ڱ�Ե���ͼ��Ļ�������Ѱ������
		//cvThreshold(gray, gray, 100, 255, CV_THRESH_BINARY_INV);
		//cvSmooth(gray, gray, 2);

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



//Function����ȡ�������ַ����Աȼ���������'���ȼ�'���� 
//Source:http://blog.csdn.net/augusdi/article/details/9000893
//Status:����OpenCV1.0�ṹ
//Improve:
//Info:[11/16/2014 jmy]

int contour4()
{
	IplImage* img = cvLoadImage("samples/contour/contour.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img_temp = cvCreateImage(cvGetSize(img), 8, 1);

	cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);

	CvMemStorage* mem_storage = cvCreateMemStorage(0);
	CvSeq *first_contour = NULL, *c = NULL;

	//////////////////////////////////////////////////////////////////////////    
	//1������һ    
	cvNamedWindow("contour1");
	cvCopyImage(img, img_temp);
	double t = (double)cvGetTickCount();

	//Ĭ�ϵ���ȡ������CV_RETR_LIST                                                                
	cvFindContours(img_temp, mem_storage, &first_contour, 88, CV_RETR_CCOMP);
	cvZero(img_temp); //�ú�   
	cvDrawContours(img_temp, first_contour, cvScalar(100), cvScalar(250), 2);

	t = (double)cvGetTickCount() - t;
	cvShowImage("contour1", img_temp);
	printf("run1 = %gms\n", t / (cvGetTickFrequency()*1000.));
	cvClearMemStorage(mem_storage);

	//////////////////////////////////////////////////////////////////////////    
	// 2��������    
	cvNamedWindow("contour2");
	cvCopyImage(img, img_temp);
	t = (double)cvGetTickCount();

	CvContourScanner scanner = cvStartFindContours(img_temp, mem_storage);
	while (cvFindNextContour(scanner));
	first_contour = cvEndFindContours(&scanner);

	cvZero(img_temp);
	cvDrawContours(img_temp, first_contour, cvScalar(100), cvScalar(100), 1);

	t = (double)cvGetTickCount() - t;
	cvShowImage("contour2", img_temp);
	printf("run2 = %gms\n", t / (cvGetTickFrequency()*1000.));
	cvClearMemStorage(mem_storage);

	cvReleaseImage(&img);
	cvReleaseImage(&img_temp);

	cvWaitKey();
	cvDestroyAllWindows();

	return 0;
	/************************************************************************/
	/* ������ run1 = 16.1431ms run2 = 15.8677ms (�ο�)
	�������Կ϶��������㷨ʱ�临�Ӷ�����ͬ��                                     */
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////    
	// �������ַ�������˶���������ȡ,������������������cvDrawContours��ʹ��    
	// ��cvDrawContours ������5������Ϊ max_level ����ICVL��������:    
	//    
	// �������������ȼ���
	// ���Ϊ0�����Ƶ�����������
	// ���Ϊ1��������������������ͬ�ļ�����������    
	// ���Ϊ2����������ͬ�����������е�һ��������������֡�
	// ���ֵΪ����������������ͬ�������������������ֱ������Ϊabs(max_level)-1����������    
	//    
	// ���źö���߳��ζ��޷����ȼ��ĺ���,���Ҳ���ʱ������>=1 ������Ч������һ��    
	// ֻ����ȡ����ʱ�����ȡģʽ��Ϊ CV_RETR_CCOMP CV_RETR_TREE ʱ���������������    
	//    
	// ����FindContours ������������������ȡģʽ(mode)������������:    
	// CV_RETR_CCOMP - ��ȡ�������������ҽ�����֯Ϊ����� hierarchy: ����Ϊ��ͨ�����Χ�߽磬�β�Ϊ�����ڲ�߽硣     
	// CV_RETR_TREE - ��ȡ���������������ع�Ƕ��������ȫ�� hierarchy     
	//     
	// �����õ�һ�ַ������в���    

	cvNamedWindow("contour_test");
	cvNamedWindow("contour_raw");
	img = cvLoadImage("samples/contour/contour.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvShowImage("contour_raw", img);
	cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);
	img_temp = cvCloneImage(img);
	cvFindContours(
		img_temp,
		mem_storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_TREE           //#1 ���������    
		);

	cvZero(img_temp);
	cvDrawContours(
		img_temp,
		first_contour,
		cvScalar(100),
		cvScalar(100),
		1                       //#2 ���������    
		);
	cvShowImage("contour_test", img_temp);
	/************************************************************************/
	/* (1, 2) = (CV_RETR_CCOMP, 1)  ��ͼ1
	(1, 2) = (CV_RETR_CCOMP, 2)  ��ͼ2
	(1, 2) = (CV_RETR_TREE, 1)   ��ͼ3
	(1, 2) = (CV_RETR_TREE, 2)   ��ͼ4
	(1, 2) = (CV_RETR_TREE, 6)   ��ͼ5
	������CV_RETR_CCOMP ֻ��ͼ���Ϊ�������,����ʹβ�,һ�ȼ�����ֻƥ��������ӽ�
	���ڲ�������2�ȼ�
	CV_RETR_TREE ��������⵽�ڰ��ȼ�1 - n ȫ������
	CV_RETR_LIST ȫ��������Ϊ1��                        */
	/************************************************************************/

	cvWaitKey();
	cvReleaseImage(&img);
	cvReleaseImage(&img_temp);
	cvReleaseMemStorage(&mem_storage);
	cvDestroyAllWindows();
	return 0;
}


//Function���Ѽ���ͼ����ж�ֵ����Ѱ�����������������
//Source: http://www.xuebuyuan.com/593448.html
//Status:
//Improve:
//Info:[11/15/2014 jmy]

int contour5()
{
	IplImage *src= cvLoadImage("samples/square/rect.png", 0);		
	if (!src){
		printf("fail to load\n");
		return -1;
	}
	cvThreshold(src, src, 128, 255, CV_THRESH_BINARY);

	cvNamedWindow("BinImg");			
	cvMoveWindow("BinImg", 60, 60);		
	cvShowImage("BinImg", src);			
	cvWaitKey();

	//-------------------------Ѱ������-------------------------------//
	int mode = CV_RETR_EXTERNAL;			       //��ȡ������ģʽ
	CvMemStorage *storage = cvCreateMemStorage(0); //����������һ���ڴ棬��ȡ����ʱ�õ�
	CvSeq *contour = 0;			                   //����һ������ָ�룬�����洢��һ���������

	//Ѱ����������,����������Ŀ
	int contours_num = cvFindContours(src, storage, &contour, sizeof(CvContour),mode, CV_CHAIN_APPROX_NONE);
	printf("count of contour:%d\n", contours_num);//���������Ŀ,��ô����600���������

	//me:ͳ��������Ŀ
	// 	int num=0;
	// 	CvSeq*contour1=contour;
	// 	while(contour1)
	// 	{
	// 		num++;
	// 		contour1=contour1->h_next;
	// 	}
	// 	printf("true count of contour:%d\n", num);//���������Ŀ,��ô����600���������


	//-------------------------��������------------------------------//
	CvSeqReader reader;					
	int count = 0;
	if (contour != 0){
		count = contour->total;			//��ȡ��������
		std::cout << "total dot count of contour =" << count << std::endl;
	}

	cvStartReadSeq(contour, &reader, 0);//��ʼ�������еĶ�ȡ����
	CvPoint pt1;						//����һ����ά�����
	IplImage* img = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);//����һ����С������ͼ��һ�����޷���3�ŵ�ͼ��ָ��
	
	cvNamedWindow("ContourImg", 1);
	cvShowImage("ContourImg", img);

	for (int i = 0; i < count; i++)
	{
		CV_READ_SEQ_ELEM(pt1, reader);				//˳��ѵ����pt1��
		cvCircle(img, pt1, 1, CV_RGB(255, 0, 0));	//����Բ������������
		cvShowImage("ContourImg", img);
		cvWaitKey(5);
	}

	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&img);

	return 0;
}