/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	main.cpp
* Brief: ͼ��任������ڣ������͸��
* Source:
* Status: 
* Date:	[4/7/2015 jmy]
************************************************************************/
#include <stdio.h>
#include "transform.h"

using namespace cv;

int main_()
{
	Mat src = imread("samples\\ts.png");

	//-----------------------------------������任��-----------------------------
	//affine();
	
	//-----------------------------------��͸�ӱ任��-----------------------------
	houghAutoRectify(src);
		
	getchar();
	return 0;
}