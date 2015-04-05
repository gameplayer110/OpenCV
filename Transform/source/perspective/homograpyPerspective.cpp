/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	homograpyPerspective.cpp
* Brief: ��͸��ƽ������ǵ���Ѱ�ұ任��ʽ��������ƥ��������о����õ�,��������Suft�ǵ����ƥ��
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/26478135
* Status: 
* Date:	[3/5/2015 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;

int homograpyPerspective()
{
	Mat img_object = imread("samples//box.png", IMREAD_GRAYSCALE);
	Mat img_scene = imread("samples//box_in_scene.png", IMREAD_GRAYSCALE);
	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; 
		return -1;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	SurfFeatureDetector detector(minHessian);
	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object from img_1 in img_2
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}


	//-----------------------------------��͸�ӱ任���֡�-----------------------------
	//����������͸��ƽ��Ѱ�ұ任��ʽ,findHomography()����ֱ��ͨ������ƽ������ƥ�������������任��ʽ��
	//֮������ֶ�ԭͼ���ĸ���Ե����б任������ͼ�ϻ�����Ӧ�ľ���
	//-----------------------------------------------------------------------
	Mat H = findHomography(obj, scene, RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0); obj_corners[1] = Point(img_object.cols, 0);
	obj_corners[2] = Point(img_object.cols, img_object.rows); obj_corners[3] = Point(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);//����ԭͼ���ĸ���Ե�㣬�õ���ͼ�е�����
	
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	Point2f offset((float)img_object.cols, 0);
	line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	waitKey(0);
	
	return 0;
}