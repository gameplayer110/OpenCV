/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	move_dete_roi.cpp
* Brief: ֡ƽ���ķ������б�����ģ
* Reference:
* Status: 
* Date:	[5/6/2015 jmy]
************************************************************************/
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
 
int bMmeanFrames(char*videoname)
{
  IplImage* pFrame = NULL; 
  IplImage* pFrImg = NULL;
  IplImage* pBkImg = NULL;
  IplImage* pROIImg = NULL;

  CvMat* pFrameMat = NULL;
  CvMat* pFrMat = NULL;
  CvMat* pBkMat = NULL;
  CvCapture* pCapture = NULL;
  int nFrmNum = 0;

  //��������
  cvNamedWindow("video", 1);
  cvNamedWindow("background",1);
  cvNamedWindow("foreground",1);
  cvNamedWindow("ROI");  


  //ʹ������������
  cvMoveWindow("video", 30, 0);
  cvMoveWindow("background", 360, 0);
  cvMoveWindow("foreground", 690, 0);

  /*������Ƶѡ��Ĳ���,��������ͷʱ�Լ��޸�*/
  
  /*
  if( argc > 2 ){
      fprintf(stderr, "Usage: �������� [video_file_name]\n");
      return -1;
    }

  //������ͷ
  if (argc ==1)
    if( !(pCapture = cvCaptureFromCAM(-1))){
	    fprintf(stderr, "Can not open camera.\n");
        return -2;
      }
 */


  //��ȡ��Ƶ�ļ�
 // if(argc == 2)
    if( !(pCapture = cvCaptureFromFile(videoname))){
        fprintf(stderr, "Can not open video file %s\n", videoname);
        return -2;
      }

  //��֡��ȡ��Ƶ
  while(pFrame = cvQueryFrame( pCapture ))
    {
      nFrmNum++;
      if(nFrmNum == 1) //����ǵ�һ֡����Ҫ�����ڴ棬����ʼ��
     {
		 pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);
		 pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);	 

		 pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		 pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		 pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

		 //ת���ɵ�ͨ��ͼ���ٴ���ע���һ֡����ǰ���ͱ������ǵ�һ֡ͼ��
		 cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
		 cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);


		 //ת����Mat������д���
		 cvConvert(pFrImg, pFrameMat);
		 cvConvert(pFrImg, pFrMat);
		 cvConvert(pFrImg, pBkMat);

    }

      else
    {
		 
		 cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
		 cvConvert(pFrImg, pFrameMat);

		 //��˹�˲��ȣ���ƽ��ͼ��
		 cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

		 //��ǰ֡������ͼ�����ǰ��ͼ��
		 cvAbsDiff(pFrameMat, pBkMat, pFrMat);

		 //��ֵ��ǰ��ͼ
		 cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);

		 //��ǰ��ͼ��������̬ѧ�˲���ȥ������ 
		 cvErode(pFrImg, pFrImg, 0, 1);
		 cvDilate(pFrImg, pFrImg, 0, 1);

		 //���±���
		 cvRunningAvg(pFrameMat, pBkMat, 0.03, 0);
		 
		 //������ת��Ϊͼ���ʽ��������ʾ
		 cvConvert(pBkMat, pBkImg);
	
             //ROI������
			 pROIImg = cvCreateImage(cvGetSize(pBkImg),IPL_DEPTH_8U, 1);  
			 cvCopy(pBkImg, pROIImg); 
			 cvRectangle(pFrame,cvPoint(30,381),cvPoint(523,433),CV_RGB(255,0,0),2);

			 CvRect omega = cvRect(30, 381, 495, 53);//�㷨����ȡ���Զ��������������ֹ��趨��  
			 cvSetImageROI(pROIImg, omega); //������copy��ͼ���Ͻ��в�����������ԭʼͼ���� 


			 //����ͼ���Ͻ��ж�ֵ��
			cvThreshold(pROIImg,pROIImg,80,255,CV_THRESH_BINARY);

 
		 //��ʾͼ��
		 cvShowImage("video", pFrame);
		 cvShowImage("background", pBkImg);
		 cvShowImage("foreground", pFrImg);
		 cvShowImage("ROI", pROIImg); 


		 //����а����¼���������ѭ��
		 //�˵ȴ�ҲΪcvShowImage�����ṩʱ�������ʾ
		 //�ȴ�ʱ����Ը���CPU�ٶȵ���
		 if( cvWaitKey(2) >= 0 )
		 break;

    }
    }


  //���ٴ���
  cvDestroyWindow("video");
  cvDestroyWindow("background");
  cvDestroyWindow("foreground");
  cvDestroyWindow("ROI");

  //�ͷ�ͼ��;���
  cvReleaseImage(&pFrImg);
  cvReleaseImage(&pBkImg);
  cvReleaseImage(&pROIImg);

  cvReleaseMat(&pFrameMat);
  cvReleaseMat(&pFrMat);
  cvReleaseMat(&pBkMat);

  cvReleaseCapture(&pCapture);

  return 0;

}


/*
 *	ʶ��ʱ���ж�,ָ�������ͼ
 * ������ͼ���ROI���Ժ����еĲ������Ƕ�ͼ���ROI������в�����
 */

//c++��ȡtxt�ļ���Ȼ���ÿ������֮������ִ洢���������񣩣���λ�ò�������ROI
