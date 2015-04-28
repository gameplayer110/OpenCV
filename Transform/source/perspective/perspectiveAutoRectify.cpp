//-----------------------------------���ı���Ŀ����Զ�͸�ӽ�����------------------------------
//	����:opencv�Դ��Ķ��ı���Ŀ����Զ�͸�ӽ���
//  �ο���csdn���ͽ��ܣ�http://blog.csdn.net/mysteryrat/article/details/8955229
//		 �ٷ����ӣ�http://opencv-code.com/tutorials/automatic-perspective-correction-for-quadrilateral-objects/
//  ״̬����ͨ�����кܴ�Ĳο���ֵ
//-------------------------------------------------------------------------
#include "rectify.h"

using namespace std;
using namespace cv;

/*
 *	���ܣ���1��͸��ͼ���Զ�У��
 *  ����������Hough�任����ԭ����4����
 *  ״̬��
 */
int houghAutoRectify(Mat src)  
{  
	//Mat src = imread("samples//rect_r.png");  
	if (src.empty()){  
		cout<<"����ͼ��ʧ��"<<endl;
		return -1;  
	}  

	Mat bw;  
	cvtColor(src,bw,CV_BGR2GRAY);  
	blur(bw,bw,Size(3,3));  
	threshold(bw, bw, 100,250,CV_THRESH_BINARY_INV);
	//Canny(bw,bw,100,100,3);  

	//����������
	Mat edge;
	bw.copyTo(edge);
	edge.setTo(0);

	vector<vector<Point> > contours;
	findContours(bw, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//for (size_t k = 0; k < contours.size(); k++)
	//	approxPolyDP(Mat(contours[k]), contours[k], 3, true);

	drawContours(edge, contours,-1,Scalar(255), 1);


	vector<Vec4i> lines;  
	HoughLinesP(bw,lines,1,CV_PI/180,70,30,10);  
	//1���طֱ�����,1�ȵĽǶȷֱ�����,>70���Լ�������,30����С�߳�,��ֱ��L�ϵĵ㣨�ҵ����֮�����С��maxLineGap=10�ģ�
	//�����߶Σ�Ȼ����Щ��ȫ��ɾ�������Ҽ�¼���߶εĲ�����������ʼ�����ֹ��  

	//ֱ�߱�ע
	for(int i = 0; i < lines.size(); i++ )
	{		
		line( src, Point(lines[i][0],lines[i][1]), Point(lines[i][2],lines[i][3]), CV_RGB(255,0,0), 1, CV_AA, 0 );
	}
	imshow("alllines",src); 


	//needed for visualization only
	//�����ǽ������ߵ������ӳ���ȫ���������ߵ�Ч������ʵ���Բ�����ô��  
	for (unsigned int i = 0;i<lines.size();i++)  
	{  
		Vec4i v = lines[i];  
		lines[i][0] = 0;  
		lines[i][1] = ((float)v[1] - v[3])/(v[0] - v[2])* -v[0] + v[1];  //ˮƽֱ��ȫ��
		
		lines[i][2] = src.cols;  
		lines[i][3] = ((float)v[1] - v[3])/(v[0] - v[2])*(src.cols - v[2]) + v[3];  

	} 


	vector<Point2f> corners;//�ߵĽ���洢  
	for (unsigned int i = 0;i<lines.size();i++)  
	{  
		for (unsigned int j=i+1;j<lines.size();j++)  
		{  
			Point2f pt = computeIntersect(lines[i],lines[j]);  
			if (pt.x >= 0 && pt.y >=0)  
			{  
				corners.push_back(pt);  
			}  
		}  
	}  

	//ֱ�߽�������
	if (corners.size()==0)
	{
		printf("no corner\n");
	}
	else
	{
		printf("corner size=%d",corners.size());
		for(int i = 0; i < corners.size(); i++ )
		{		
			line( bw, Point(0,0), Point(corners[i].x,corners[i].y), CV_RGB(0,255,0), 1, CV_AA, 0 );
		}
		imshow("corner_line",bw);
	}
	
	//�����ı������
	vector<Point2f> approx;  
	approxPolyDP(Mat(corners),approx,arcLength(Mat(corners),true)*0.02,true);  

	if (approx.size()!=4)  //��ϳ��Ķ���α������ı���
	{  
		cout<<"The object is not quadrilateral���ı��Σ�!"<<endl;  
		return -1;  
	}  

	//get mass center  
	for (unsigned int i = 0;i < corners.size();i++)  
	{  
		center += corners[i];  //����
	}  

	center *=(1./corners.size());  
	sortCorners(corners,center); //ѡ�����4����
	Mat dst = src.clone();  

	//Draw Lines  
	for (unsigned int i = 0;i<lines.size();i++)  
	{  
		Vec4i v = lines[i];  
		line(dst,Point(v[0],v[1]),Point(v[2],v[3]),CV_RGB(0,255,0));  //Ŀ���黭����   
	}  

	//draw corner points  
	circle(dst,corners[0],3,CV_RGB(255,0,0),2);  
	circle(dst,corners[1],3,CV_RGB(0,255,0),2);  
	circle(dst,corners[2],3,CV_RGB(0,0,255),2);  
	circle(dst,corners[3],3,CV_RGB(255,255,255),2);  

	//draw mass center  
	circle(dst,center,3,CV_RGB(255,255,0),2);  

	Mat quad = Mat::zeros(300,220,CV_8UC3);//�趨У������ͼƬ��320*240��Ϊ300*220  

	//corners of the destination image  
	vector<Point2f> quad_pts;  
	quad_pts.push_back(Point2f(0,0));  
	quad_pts.push_back(Point2f(quad.cols,0));//(220,0)  
	quad_pts.push_back(Point2f(quad.cols,quad.rows));//(220,300)  
	quad_pts.push_back(Point2f(0,quad.rows));  

	// Get transformation matrix  
	//��Դ����ϵcoreners���ѻ���ģ���Ŀ������ϵquad_pts��ת������  
	Mat transmtx = getPerspectiveTransform(corners,quad_pts);   

	// Apply perspective transformation͸��ת��  
	warpPerspective(src,quad,transmtx,quad.size());  
	
	imshow("src",src);  
	imshow("image",dst);  
	imshow("quadrilateral",quad);  

	waitKey();  
	getchar();
	return 0;  
}  

/*
 *	���ܣ���2��͸��ͼ���Զ�У��
 *  ������������С��Ӿ��β���ԭʼ���ĸ���
 *  ״̬��
 */
int rectAutoRectify(Mat src)
{
	//-----------------------------------����Դͼ��Ĵ���-----------------------------
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	threshold(gray, gray, 50, 255, CV_THRESH_BINARY_INV);
	
	//(1) ���Һͻ���������
	vector<vector<Point>> contours;
	findContours(gray,contours, CV_RETR_EXTERNAL,	CV_CHAIN_APPROX_NONE); 
	if (contours.size()>1)	{
		cout << "ERROR" << endl;
		return -1;
	}

	//(2) ����С��Ӿ��β�������ת��
	RotatedRect box = minAreaRect(Mat(contours[0]));
	Point2f  vtx[4];
	box.points(vtx);
	for (int i = 0; i < 4; i++) //������С��Ӿ���
		line(src, vtx[i], vtx[(i + 1) % 4], Scalar(144, 255, 22), 1, CV_AA);
	cout << "Rotated Angle: " << box.angle << endl;
	
	vector<Point2f> corners;
	corners.push_back(vtx[0]);
	corners.push_back(vtx[1]);
	corners.push_back(vtx[2]);
	corners.push_back(vtx[3]);

	//draw corner of src points  
	circle(src, corners[0], 3, CV_RGB(255, 0, 0), 2);
	circle(src, corners[1], 3, CV_RGB(0, 255, 0), 2);
	circle(src, corners[2], 3, CV_RGB(0, 0, 255), 2);
	circle(src, corners[3], 3, CV_RGB(255, 255, 255), 2);


	//-----------------------------------����Ŀ��ͼ��Ĵ���-----------------------------
	Mat quad = Mat::zeros(300, 220, CV_8UC3);//�趨У������ͼƬ��320*240��Ϊ300*220  
	//corners of the destination image 
	vector<Point2f> quad_pts;
	quad_pts.push_back(Point2f(0, 0));
	quad_pts.push_back(Point2f(quad.cols, 0));//(220,0)  
	quad_pts.push_back(Point2f(quad.cols, quad.rows));//(220,300)  
	quad_pts.push_back(Point2f(0, quad.rows));

	// Get transformation matrix����Դ����ϵcoreners���ѻ���ģ���Ŀ������ϵquad_pts��ת������  
	Mat transmtx = getPerspectiveTransform(corners, quad_pts);   

	// Apply perspective transformation͸��ת��  
	warpPerspective(src, quad, transmtx, quad.size());

	imshow("src",src);  
	imshow("quadrilateral", quad);

	waitKey();

	return -1;

}