/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	contour.cpp
* Brief: ���ļ��������������������Լ���ͼ��Ȼ��������
* Source:http://blog.csdn.net/MoreWindows
* Status: 
* Date:	[1/14/2015 jmy]
************************************************************************/


#include <opencv2/opencv.hpp>
using namespace std;

/*
 *	���ܣ��Լ���ͼ��Ȼ�����������⣬��Ҫע��ļ�������
 *      ��1����OpenCV��findContour��������Ѱ�ҵ��ǰ�ɫ����Ϊ����
 *  �����
 *  ״̬��
 */

int contour1()
{	
	const char *pstrWindowsSrcTitle = "ԭͼ(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsOutLineTitle = "����ͼ(http://blog.csdn.net/MoreWindows)";

	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = 200;

	// ����ͼ��
	IplImage *pSrcImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	//cvRectangle(pSrcImage, cvPoint(0, 0), cvPoint(pSrcImage->width, pSrcImage->height),
		       // CV_RGB(255, 255, 255), CV_FILLED);// ���ɰ�ɫ
	cvSetZero(pSrcImage);
	
	// ��Բ
	CvPoint ptCircleCenter = cvPoint(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	int nRadius = 80;
	cvCircle(pSrcImage, ptCircleCenter, nRadius, CV_RGB(255, 255, 255), CV_FILLED);
	ptCircleCenter = cvPoint(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	nRadius = 30;
	cvCircle(pSrcImage, ptCircleCenter, nRadius, CV_RGB(0, 0, 0), CV_FILLED);
	
	// ������
	CvPoint ptLeftTop = cvPoint(IMAGE_WIDTH / 2 + 20, 20);
	CvPoint ptRightBottom = cvPoint(IMAGE_WIDTH - 20, IMAGE_HEIGHT - 20);
	cvRectangle(pSrcImage, ptLeftTop, ptRightBottom, CV_RGB(255, 255, 255), CV_FILLED);
	ptLeftTop = cvPoint(IMAGE_WIDTH / 2 + 60, 40);
	ptRightBottom = cvPoint(IMAGE_WIDTH - 60, IMAGE_HEIGHT - 40);
	cvRectangle(pSrcImage, ptLeftTop, ptRightBottom, CV_RGB(0, 0, 0), CV_FILLED);
	
	// ��ʾԭͼ
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);


	// תΪ�Ҷ�ͼ
	IplImage *pGrayImage =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	// תΪ��ֵͼ
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(pGrayImage), IPL_DEPTH_8U, 1);
	cvThreshold(pGrayImage, pBinaryImage, 250, 255, CV_THRESH_BINARY);


	// �������������ؼ�⵽�������ĸ���
	CvMemStorage *pcvMStorage = cvCreateMemStorage();
	CvSeq *pcvSeq = NULL;
	/*Ҫ�޸ĵ�Ҳ��������ط�*/
	cvFindContours(pBinaryImage, pcvMStorage, &pcvSeq, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// ������ͼ
	IplImage *pOutlineImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 3);
	cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height), 
		        CV_RGB(255, 255, 255), CV_FILLED);// ���ɰ�ɫ
	int nLevels = 3;
	/*Ҫ�޸ĵ�Ҳ��������ط�,���죬�ڲ���*/
	cvDrawContours(pOutlineImage, pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), nLevels, 2);
	
	
	// ��ʾ����ͼ
	cvNamedWindow(pstrWindowsOutLineTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsOutLineTitle, pOutlineImage);
	cvWaitKey(0);
	cvReleaseMemStorage(&pcvMStorage);
	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsOutLineTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pBinaryImage);
	cvReleaseImage(&pOutlineImage);
	return 0;
}




/*
 *	���ܣ�������⣬����˻�����
 *  ���룺
 *  �����
 *  ״̬��
 */
IplImage *greyimg = NULL;
const char *binWndName = "��ֵͼ(http://blog.csdn.net/MoreWindows)";
const char *contourWndName = "����ͼ(http://blog.csdn.net/MoreWindows)";
CvSeq *g_pcvSeq = NULL;

void contour2_trackbar(int n_level)
{
	// תΪ��ֵͼ
	IplImage *pBinaryImage = cvCreateImage(cvGetSize(greyimg), IPL_DEPTH_8U, 1);
	cvThreshold(greyimg, pBinaryImage, 50, 255, CV_THRESH_BINARY);
	// ��ʾ��ֵͼ
	cvShowImage(binWndName, pBinaryImage);

	CvMemStorage* cvMStorage = cvCreateMemStorage();
	// �������������ؼ�⵽�������ĸ���
	cvFindContours(pBinaryImage,cvMStorage, &g_pcvSeq,sizeof(CvContour),CV_RETR_EXTERNAL);

	IplImage *pOutlineImage = cvCreateImage(cvGetSize(greyimg), IPL_DEPTH_8U, 3);
	//int _levels = 3;
	cvZero(pOutlineImage);
	cvDrawContours(pOutlineImage, g_pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), n_level-3);
	cvShowImage(contourWndName, pOutlineImage);

	cvReleaseMemStorage(&cvMStorage);
	cvReleaseImage(&pBinaryImage);
	cvReleaseImage(&pOutlineImage);
}

int contour2(char*path)
{	
	const char *pstrWindowsSrcTitle = "ԭͼ(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBarName = "level";

	IplImage *pSrcImage = cvLoadImage(path, CV_LOAD_IMAGE_UNCHANGED);
	if (!pSrcImage){
		printf("����ʧ��\n");
		exit(0);
	}
	// ��ʾԭͼ
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsSrcTitle, pSrcImage);

	// תΪ�Ҷ�ͼ
	greyimg =  cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, greyimg, CV_BGR2GRAY);

	// ������ֵͼ������ͼ����
	cvNamedWindow(binWndName, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(contourWndName, CV_WINDOW_AUTOSIZE);

	// ������	
	int nThreshold = 0;
	cvCreateTrackbar(pstrWindowsToolBarName, binWndName, &nThreshold, 7, contour2_trackbar);
	contour2_trackbar(1);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(binWndName);
	cvDestroyWindow(contourWndName);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&greyimg);
	return 0;
}

//���ܲ�����
//int main()
//{
//	//contour1();
//	contour2("samples/contour/contour.jpg");
//	return 0;
//}