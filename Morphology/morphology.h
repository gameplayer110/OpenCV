/************************************************************************/
/* ����ͷ����                                                    
/************************************************************************/
#include <string>
#include <opencv2/opencv.hpp>

int morphologyShow();//������ʾ
int morphy_whole(std::string filename);//������ʾ��������ñ�ͺ�ñ�任

int erode_dilate();
int open_close();
int morphy_edge();//��Ե���
int morphy_corner();//�ǵ���
int morphy_apply_location();//��λ

//------------------------------------ͼ��ϸ��-----------------------------
int thin(char*input);

