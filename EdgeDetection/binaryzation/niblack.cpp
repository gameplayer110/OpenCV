/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	niblack.cpp
* Brief: Niblack�㷨��OpenCV2ʵ��
* Source:http://download.csdn.net/download/u012033076/7159543
* Status: 
* Date:	[3/14/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>

using namespace cv;
//using namespace std;

//����Win_width���ڿ�ȣ�Win_height���ڸ߶ȣ�kΪ����ϵ����ͨ��ȡ-0.1��
void Niblack(Mat &srcImg, Mat &binImg, int Win_width, int Win_height, double k)
{
	srcImg.convertTo(srcImg, CV_64F);
	int width = srcImg.cols;
	int height = srcImg.rows;
	binImg.create(height, width, CV_64FC1);
	for(int j = 0; j < height; j++)
	{
		double *srcptr = srcImg.ptr<double>(j);
		double *dstptr = binImg.ptr<double>(j);
		for(int i = 0; i < width; i++)
		{
			int begin_y = j - Win_height, begin_x = i - Win_width;
			int end_y = j + Win_height, end_x = i + Win_width;
			if(begin_y < 0) begin_y = 0;
			if(begin_x < 0) begin_x = 0;
			if(end_y >= height) end_y = height-1;
			if(end_x >= width ) end_x = width -1;
			int total = ( end_y - begin_y +1 ) * ( end_x - begin_x +1 );  //�ô������ܵ����ص���

			double mean = 0;//��ֵ
			double std  = 0;//��׼��
			
			for(int y = begin_y; y<end_y; y++)
			{
				double *ptrWin = srcImg.ptr<double>(y); /*(char*)(srcImg->imageData + srcImg->widthStep * y);*/
				for(int x = begin_x; x < end_x; x++)
				{
					mean += ptrWin[x];
				}
			}  //�����ڸ�С�����ڻҶ�ֵ�ܺ�

			mean = mean/total;  //�ô��ڵ�ƽ���Ҷ�
			double sq;
			for(int y = begin_y; y < end_y; y++)
			{
				double *ptrWin = srcImg.ptr<double>(y);
				for(int x = begin_x; x < end_x; x++)
				{
					sq = (mean - ptrWin[x]) * (mean - ptrWin[x]);
					std += sq;
				}
			}   
			//������������ܵķ����
			std /=total;
			std = sqrt(std);  //�����׼��
			double threshold = mean + k * std;  //������õ��ĵ���ֵ

			if(srcptr[i] > threshold)
				dstptr[i] = 255;
			else
				dstptr[i] = 0;
		}
	}
}

//���ܲ�����
int adaptiveThreshold(char*path);

//int main(int argc, char* argv[])
//{
//	Mat src, dst;
//	src = imread("C:\\Users\\jmy\\Desktop\\1.bmp");
//	cvtColor(src, src, CV_RGB2GRAY); 
//	medianBlur(src, src, 3);
//
//	Niblack(src, dst, 15, 15, -0.1);
//
//	src.convertTo(src, CV_8UC1);
//	dst.convertTo(dst, CV_8UC1);
//
//	medianBlur(dst, dst, 3);	
//	imshow("dst", dst);
//   	imshow("src", src);
//	adaptiveThreshold("C:\\Users\\jmy\\Desktop\\1.bmp");
//
//	waitKey(0);
//	return 0;
//}

