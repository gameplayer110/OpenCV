/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	wellner93.cpp
* Brief: 
* Reference:http://blog.csdn.net/xgmiao/article/details/9320667
* Status: 
* Date:	[5/7/2015 jmy]
************************************************************************/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void quickAdaptiveThreshold(unsigned char* grayImage, unsigned char* binImage, int width, int height)
{

	int t = 15;
	int s = width >> 3;
	const int T = 9; //T�Ǳ��⸡��������
	const int power2S = 1 << T; //ͬʱ�˳�2��T�η�,���⸡������

	int factor = power2S * (100 - t) / (100 * s);
	int gn = 127 * s;
	int q = power2S - power2S / s; //�ȱ����б���
	int pn, hn;
	unsigned char *scanline = NULL;
	int *prev_gn = NULL;

	prev_gn = new int[width];
	for (int i = 0; i < width; i++)
	{
		prev_gn[i] = gn;
	}

	//���ҽ���ɨ��������
	for (int y = 0; y < height; y++)
	{
		int yh = y * width;
		scanline = grayImage + y * width;
		for (int x = 0; x < width; x++)    //���д�������ɨ��
		{
			pn = scanline[x];
			gn = ((gn * q) >> T) + pn;
			hn = (gn + prev_gn[x]) >> 1;     //����������ƽ��
			prev_gn[x] = gn;
			pn < (hn*factor) >> T ? binImage[yh + x] = 0 : binImage[yh + x] = 0xff;
		}
		y++;
		if (y == height)
		{
			break;
		}
		yh = y * width;
		scanline = grayImage + y * width;
		for (int x = width - 1; x >= 0; x--)  //���д�������ɨ��
		{
			pn = scanline[x];
			gn = ((gn * q) >> T) + pn;
			hn = (gn + prev_gn[x]) >> 1;
			prev_gn[x] = gn;
			pn < (hn*factor) >> T ? binImage[yh + x] = 0 : binImage[yh + x] = 0xff;
		}
	}

	delete prev_gn;
}


//���ܲ�����
//int main(int argc,char**argv)
//{
//	Mat gray = imread("E:\\360YPan\\samples\\textdete\\137.jpg", 0);
//	Mat bin;
//	gray.copyTo(bin);
//
//	quickAdaptiveThreshold(gray.data, bin.data, gray.cols, gray.rows);
//	imshow("Bin", bin);
//	waitKey();
//
//	return 0;
//}