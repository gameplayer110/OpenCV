//������ȡ�����������
#include <iostream>
#include "Feature.h"

using namespace std;

int main_()
{
	char*filename="samples/firecorner.jpg";
	Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	
	//garbor(filename);

	

	
	cout<<"====���������========="<<endl;
	cin.get();
	return 0;
}