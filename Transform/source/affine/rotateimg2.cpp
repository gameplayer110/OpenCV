/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	rotateimg.cpp
* Brief: OpenCV2.0ʵ��ͼ����ת������
* Source:http://blog.csdn.net/masibuaa/article/details/9017761
* Status: �ؼ��ǽ�����ͼ�����ţ�ʹ�ñ任���ͼ��������
* Date:	[3/28/2015 jmy]
************************************************************************/
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define SCALE 1   //���ű���

int rotate2()
{
	Mat src = imread("samples//rect.png");
	Mat dst;

	int angle = -30;//��ת�Ƕ�(��ֵ��ʾ��ʱ����ת)
	int length;//���ͼ��Ŀ�Ȼ�߶ȣ����ͼ������Ϊ������
	
	//���������С(SCALE<=1)�������ᵼ����ʱͼ���зŲ���ԭͼ�����Զ���������С����������ͼ�����ʱͼ��ĳ�����Ϊԭͼ�ĶԽ��߳���
	if (SCALE <= 1)
		length = sqrt(src.cols*src.cols + src.rows*src.rows);
	else //Ϊ�˱�֤���������ת���ܷ��£����ͼ��ĳ��Ϳ���Ϊ����ͼ��Խ��߳��ȳ���SCALE
		length = sqrt(src.cols*src.cols + src.rows*src.rows) * SCALE;
	
	//������ʱͼ�񣬳�����ͼ��ĶԽ��߳��ȣ���Դͼ���Ƶ���ʱͼ������ĺ��ٱ任
	Mat tempImg(length, length, src.type(),Scalar(255,0,0));//��ʱͼ�񣬴�С�����ͼ��һ����
	int ROI_x = length / 2 - src.cols / 2;//ROI�������Ͻǵ�x����
	int ROI_y = length / 2 - src.rows / 2;//ROI�������Ͻǵ�y����
	Rect ROIRect(ROI_x, ROI_y, src.cols, src.rows);//ROI����
	rectangle(tempImg, ROIRect, Scalar(0, 0, 255), 1);
	Mat tempImgROI2(tempImg, ROIRect);//tempImg���м䲿��
	src.copyTo(tempImgROI2);//��ԭͼ���Ƶ�tempImg������

	Point2f center(length / 2, length / 2);//��ת����
	circle(tempImg, center, 2, Scalar(0, 255, 0), 2);
	Mat M = getRotationMatrix2D(center, angle, SCALE);//������ת�ķ���任����

	//�����������ľ�����ʲô
	cout << "�任����" << endl;
	cout << M.at<double>(0, 0) << "," << M.at<double>(0, 1) << "," << M.at<double>(0, 2) << "," << endl;
	cout << M.at<double>(1, 0) << "," << M.at<double>(1, 1) << "," << M.at<double>(1, 2) << "," << endl;

	warpAffine(tempImg, dst, M, Size(length, length));//����任
	
	//��ȡĿ��ͼ��
	Mat rst;
	getRectSubPix(dst, Size(src.rows, src.cols), center, rst);

	//��ʾ
	imshow("src", src);
	imshow("tempImg", tempImg);
	imshow("dst", dst);
	imshow("rst", rst);

	waitKey(0);
	return 0;
}

//���ܲ�����
int main_(int argc,char**argv)
{
	rotate2();

    std::cin.get();
	return 0;
}