/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	transform.cpp
* Brief: �任ͷ�ļ�
* Source:
* Status: 
* Date:	[4/7/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

//-----------------------------------�����䡿-----------------------------
int handPerspective(std::string filename);

//-----------------------------------��͸�ӡ�-----------------------------

//�ֶ�У��
int handPerspective(std::string filename);

//���õ�Խ���͸��У��
int houghAutoRectify(cv::Mat src);
int rectAutoRectify(cv::Mat src);

//����͸��ƽ��Ѱ�ұ任��ʽ
int homoPlaneAutoRectify();
