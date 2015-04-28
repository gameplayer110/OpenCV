/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	shapeDescriptor2.cpp
* Brief: ������״�����Ӽ��㣬OpenCV2.0�ṹ
* Source:http://www.cnblogs.com/wsc36305/archive/2012/10/15/2725218.html
* Status: 
* Date:	[4/6/2015 jmy]
************************************************************************/

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int shapeDescriptor2()
{
	 string filename = "0_9.png";
	 Mat image= imread(filename,0);
	 if (!image.data)   return 0; 
	 threshold(image, image, 10, 255, THRESH_BINARY_INV);
	 namedWindow("Binary Image");
	 imshow("Binary Image",image);

	//(1) ���Һͻ���������
	 vector<vector<Point>> contours;
	 findContours(image,  //findContours�������Ƕ�ֵͼ��
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	 // draw contours on the original image
	 Mat original = imread(filename);
	 drawContours(original, contours,
		 -1, // draw all contours
		 Scalar(255, 0, 0), // in blue
		 2); // with a thickness of 2

	 namedWindow("Contours on Animals");
	 imshow("Contours on Animals", original);


	//��2�������������ʽ���ͳ��
	// �����ĳ���
	cout << "ContoursNum: " << contours.size() << endl; //�����ĸ���
	vector<vector<Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours) {
		cout << "ContourLength: " << itContours->size() << endl;//ÿ�����������ĵ���
		cout << "ContourLength1 " << arcLength(*itContours,1) << endl;
	}

	// Eliminate too short or too long contours
	int cmin= 100;  // minimum contour length
	int cmax= 1000; // maximum contour length
	vector<vector<Point>>::const_iterator itc= contours.begin();
	while (itc!=contours.end()) {
		if (itc->size() < cmin || itc->size() > cmax)
			itc= contours.erase(itc);
		else 
			++itc;
	}

	//���������
	double area;
	double amin = 100, amax = 10000;
	vector<vector<Point>>::const_iterator ita = contours.begin();
	while (ita != contours.end())
	{
		area = contourArea(*ita);
		cout << "Area: " << area<< endl;

		if (area < amin || area > amax)
			ita = contours.erase(ita);
		else
			++ita;
	}



	//��3��������������״������
	// draw white contours on black image
	Mat result(image.size(), CV_8UC3, Scalar(0));
	drawContours(result, contours,     
		-1, // draw all contours
		Scalar(255,255,255), // in white
		1); // with a thickness of 1

	namedWindow("ExternalContours");
	imshow("ExternalContours", result);


	// testing the bounding box (blue)
	Rect r0= boundingRect(Mat(contours[0]));
	rectangle(result,r0,Scalar(255,0,0),1);

	// testing the enclosing circle (green)
	float radius;
	Point2f center;
	minEnclosingCircle(Mat(contours[1]),center,radius);
	circle(result,Point(center),static_cast<int>(radius),Scalar(0,255,0),1);

	//testing the ellipse(red),��Բ���
	RotatedRect rrect= fitEllipse(Mat(contours[2]));
	ellipse(result,rrect,Scalar(0,0,255),2);
	Point2f  vtxc[4];
	rrect.points(vtxc);
	for (int i = 0; i < 4; i++) //���������Բ����Ӿ���
		line(result, vtxc[i], vtxc[(i + 1) % 4], Scalar(23, 255, 12), 1, CV_AA);
	cout << "EllipseBoxAngle:" << rrect.angle << endl;  //�����Բ�ĽǶ�
	cout << "EllipseBoxArea:" << rrect.size.width*rrect.size.height << endl; //�����Բ�����о��ε����
	Rect ellipseBox = rrect.boundingRect();
	rectangle(result, ellipseBox, Scalar(255, 4, 23), 1, 8);


	
	//testing the approximate polygon
	vector<Point> poly;
	approxPolyDP(Mat(contours[2]),poly,5,true);
	cout << "Polygon size: " << poly.size() << endl;
	vector<Point>::const_iterator itp= poly.begin();//Iterate over each segment and draw it��������Ӷ����
	while (itp!=(poly.end()-1)) {
		line(result,*itp,*(itp+1),Scalar(255,0,255),2);
		++itp;
	}
	line(result, *(poly.begin()), *(poly.end() - 1), Scalar(255, 0, 255), 2);// last point linked to first point

	// testing the convex hull
	vector<Point> hull;
	convexHull(Mat(contours[3]),hull);
	vector<Point>::const_iterator it= hull.begin();// Iterate over each segment and draw it������convexHull͹��
	while (it!=(hull.end()-1)) {
		line(result,*it,*(it+1),Scalar(0,234,255),2);
		++it;
	}
	line(result,*(hull.begin()),*(hull.end()-1),Scalar(255),2);// last point linked to first point

	
	//testing the moments��������������
	itc= contours.begin();
	while (itc!=contours.end()) {
		Moments mom= moments(Mat(*itc++));// compute all moments
		// draw mass center
		circle(result,
			// position of mass center converted to integer
			Point(mom.m10/mom.m00,mom.m01/mom.m00),
			2,Scalar(122,22,255),2); 
	}

	//test miniBoundingBox,����С��Ӿ���
	RotatedRect box = minAreaRect(Mat(contours[2]));
	Point2f  vtx[4];
	box.points(vtx);
	for (int i = 0; i < 4; i++) //������С��Ӿ���
		line(result, vtx[i], vtx[(i + 1) % 4], Scalar(144, 255, 22), 1, CV_AA);
	cout << "Rotated Angle: " << box.angle << endl;



	namedWindow("Some Shape descriptors");
	imshow("Some Shape descriptors",result);


	waitKey();
	return 0;
}