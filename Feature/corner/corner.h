#include <opencv2/opencv.hpp>
//-----------------------------------���ǵ��⡿-----------------------------
//�������ǵ���
//-----------------------------------------------------------------------
int fast_corner_detect(char*filename);
int fast_feature_opencv(char*filename);
int harris_corner_detect(char*filename);
int harris_corner_detect_improve(char*filename);
int sift_feature();
int susan_corner_detect(char*filename);
int surf_feature();
int surf_feature_opencv();//�ó�����ʾ��ģ��ƥ��
void CheckFireAngle(cv::Mat &img);//���ּ�Ǽ��