//������ȡ�����������
#include <iostream>
#include "Feature.h"

using namespace std;

int main()
{
	char*filename="samples/firecorner.jpg";
	Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	//garbor(filename);
	//-----------------------------------���ǵ����ƥ�䡿-----------------------------
	//surf_feature();
	//sift_feature();
	//surf_feature_opencv();
	//-----------------------------------���ǵ��⡿-----------------------------
	//harris_corner_detect(filename);
	//susan_corner_detect(filename);
	//fast_corner_detect(filename);
	threshold(img,img,50,200,THRESH_BINARY);
	CheckFireAngle(img);


	cout<<"====���������========="<<endl;
	cin.get();
	return 0;
}