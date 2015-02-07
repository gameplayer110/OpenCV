/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	mserArea.cpp
* Brief: ͼ�������ȶ���ֵ�������ȡ
* Source:http://download.csdn.net/download/czx19890592/8244321
* Reference:http://blog.csdn.net/zhaocj/article/details/40742191
* Status: 
* Date:	[1/27/2015 jmy]
************************************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <io.h>

using namespace cv;
using namespace std;

static const Vec3b bcolors[] =
{
	Vec3b(0, 0, 255),
	Vec3b(0, 128, 255),
	Vec3b(0, 255, 255),
	Vec3b(0, 255, 0),
	Vec3b(255, 128, 0),
	Vec3b(255, 255, 0),
	Vec3b(255, 0, 0),
	Vec3b(255, 0, 255),
	Vec3b(255, 255, 255)
};

/*
 *	���ܣ�������ͼ�������ȶ���ֵ����
 *  ������
 *  ״̬��
 */
int mserArea(string filename)
{
	Mat img0, img, yuv, gray, result;
	img0 = imread(filename, 1);
	resize(img0, img0, Size(), 0.5, 0.5); //�ߴ�����Ϊԭ����һ��
	cvtColor(img0, yuv, COLOR_BGR2YCrCb);
	cvtColor(img0, gray, COLOR_BGR2GRAY);
	cvtColor(gray, img, COLOR_GRAY2BGR);
	img.copyTo(result);

	vector<vector<Point> > contours;
	double t = (double)getTickCount();
	MSER()(gray, contours); //����MSER��ȡ������ȶ���ֵ���������
	t = (double)getTickCount() - t;
	printf("MSER extracted %d contours in %g ms.\n", (int)contours.size(), t*1000. / getTickFrequency());

	// draw mser's with different colors
	for (int i = (int)contours.size() - 1; i >= 0; i--)
	{
		
		//drawContours(result, contours[i], i, CV_RGB(0, 255, 0), -1, 8, INT_MAX);//��������,������
		//�����Ƶ�����,��ֵ����ڲ�Ҳ��������
		const vector<Point>& r = contours[i]; 
		for (int j = 0; j < (int)r.size(); j++)
		{
			Point pt = r[j];
			img.at<Vec3b>(pt) = bcolors[i % 9];
		}

		// find ellipse (it seems cvfitellipse2 have error or sth?)
		RotatedRect box = fitEllipse(r); //������(һϵ�е�)������Բ���
		box.angle = (float)CV_PI / 2 - box.angle;
		//ellipse(result, box, Scalar(0, 0, 255), 1);

		//������ת����
		Point2f pt[4];
		box.points(pt);
		for (int i = 0; i < 4; ++i)
		{
			line(result, cvPointFrom32f(pt[i]), cvPointFrom32f(pt[((i + 1) % 4) ? (i + 1) : 0]),CV_RGB(255, 0, 0), 1);
		}
	}


	//imshow("original_img0", img0);
	//imshow("original_yuv", yuv);
	imshow("response_gray2color", img);
	imshow("result", result);
	//imwrite("response_gray2color.jpg", img);
	//imwrite("result.jpg", result);
	
	waitKey(0);
	return 0;
}

/*
 *	���ܣ������ļ������ļ����ڵ��ļ�
 *  ������
 *  ״̬��
 */
void getFiles(string path, vector<string>& files)
{
	//�ļ����
	long  hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.JPG").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮,�������,�����б�
			if ((fileinfo.attrib &_A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

//���ܲ�����
int main()
{
	//Mat src = imread("");
	//mserArea("samples//200.png");
    //�����ķ�ʽ

	string path = "G:\\DataSET\\TextDetection\\MSRA-TD500\\test";
	vector<string> filenames;
	getFiles(path, filenames);
	for (vector<string>::iterator it = filenames.begin(); it < filenames.end();it++)
	{
		cout << *it << endl;
		mserArea(*it);
		waitKey();
	}

	return 0;
}

