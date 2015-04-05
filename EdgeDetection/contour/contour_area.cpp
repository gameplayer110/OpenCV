/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	contour_demo.cpp
* Brief: �������ֶ�����������������Ͷ������ϵ����
* Source:
* Status: ���
* Date:	[3/7/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace  cv;
using namespace std;

int contor_demo()
{
	vector<Point>  contour;
	Mat src(50, 50, CV_8UC3,Scalar(0));

	contour.push_back(Point2f(0, 0));
	contour.push_back(Point2f(10, 0));
	contour.push_back(Point2f(10, 10));
	contour.push_back(Point2f(5, 4));

	//circle(src, Point2f(0, 0), 1, Scalar(0, 0, 255), CV_FILLED, CV_AA);
	//circle(src, Point2f(10, 0), 1, Scalar(0, 0, 255), CV_FILLED, CV_AA);
	//circle(src, Point2f(10, 10), 1, Scalar(0, 0, 255), CV_FILLED, CV_AA);
	//circle(src, Point2f(5, 0), 1, Scalar(0, 0, 255), CV_FILLED, CV_AA);

	
	double  area0 = contourArea(contour);
	vector<Point>  approx;
	approxPolyDP(contour, approx, 5, true);//��������
	double  area1 = contourArea(approx);
	cout << "���������  =" << area0 << endl <<
		    "������϶�������  =" << area1 << endl <<
		    "approx poly vertices=" << approx.size() << endl;

	//���Ƶ�������
	for (vector<Point>::iterator it = contour.begin(); it != contour.end(); it++)
	{
		circle(src, Point2d(it->x,it->y), 1, Scalar(255, 0, 0), CV_FILLED, CV_AA);
	}
	resize(src, src, Size(), 3, 3);
	imshow("src", src);

	waitKey();
	return 0;
}

//���ܲ�����
//int main(int argc, char**argv)
//{
//	contor_demo();
//
//	std::cin.get();
//	return 0;
//}