/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	square_merge.cpp
* Brief: ���ο��ں��㷨
* Source:http://www.tuicool.com/articles/7zqUJr6
* Status: ���ڴ������⣬δ���
* Date:	[3/22/2015 jmy]
************************************************************************/
#include<opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

/*-----------------------MeanShift���ֲ��ģ�Դ�����Ƕ�ȫ�֣�------------------------*/
//meanshift �ں�
class MeanshiftGrouping
{
public:
	//	 MeanshiftGrouping msGrouping(smothing, hits,rectList, hitWeights, 1e-5, 100);//�õ�
	///////////////////////////////////////////////////////////////////////
	//	 msGrouping.getModes(resultHits, resultWeights, 1);
	//////////////////////////////////////////////////////////////////////
	MeanshiftGrouping(const Point3d& densKer, const vector<Point3d>& posV, const vector<Rect>&list,
		const vector<double>& wV, double eps, int maxIter = 20)
	{
		densityKernel = densKer;
		weightsV = wV;
		positionsV = posV;
		positionsCount = (int)posV.size();
		meanshiftV.resize(positionsCount);
		distanceV.resize(positionsCount);
		iterMax = maxIter;
		modeEps = eps;

		for (unsigned i = 0; i < positionsV.size(); i++)
		{
			meanshiftV[i] = getNewValue(positionsV[i], list[i], list);//positionV ֻ���е�����û�г���
			distanceV[i] = moveToMode(meanshiftV[i], list[i], list);//�����ΪiterMax��ѭ��//��ֵƯ�ƺ��ֵ
			meanshiftV[i] -= positionsV[i];//��һ������û�õ�
		}
	}
	void getModes(vector<Point3d>& modesV, vector<double>& resWeightsV, const double eps)
	{
		for (size_t i = 0; i < distanceV.size(); i++)
		{
			bool is_found = false;
			for (size_t j = 0; j < modesV.size(); j++)
			{
				if (getDistance(distanceV[i], modesV[j]) < eps)//ŷʽ����С����ֵ
				{
					is_found = true;
					break;
				}
			}
			if (!is_found)
			{
				modesV.push_back(distanceV[i]);//��Ӿ���ϴ�ĵ㣬Ҳ����˵���������ϴ󣬲���ͬһ�����ο�
			}
		}

		resWeightsV.resize(modesV.size());

		for (size_t i = 0; i < modesV.size(); i++)
		{
			resWeightsV[i] = getResultWeight(modesV[i]);//�õ����Ȩֵ
		}
	}

protected:
	vector<Point3d> positionsV;
	vector<double> weightsV;

	Point3d densityKernel;
	int positionsCount;

	vector<Point3d> meanshiftV;
	vector<Point3d> distanceV;
	int iterMax;
	double modeEps;

	Point3d getNewValue(const Point3d& inPt, const Rect inR, const vector<Rect>list) const
	{//inPt ������ά���� ��inR Ϊ����ľ���  listΪ���о���
		Point3d resPoint(.0);
		Point3d ratPoint(.0);
		int value = 20;//ֻ���Ǿ��ο��ĸ��ǲ�ֵС��20�ĵ㣬��������Լ��趨
		for (size_t i = 0; i < positionsV.size(); i++)
		{
			Point3d aPt = positionsV[i];
			// double rate = exp(aPt.z);
			if (inR.x - list[i].x > value || inR.y - list[i].y > value || inR.width - list[i].width > value || inR.height - list[i].height > value)
				continue;//�ֲ��жϣ��������ͬһ�����θ��������ų��������ε�Ӱ��

			Point3d bPt = inPt;
			Point3d sPt = densityKernel;//��


			////////////////////////////////////////
			sPt.x *= exp(aPt.z);//ZΪ�߶�
			sPt.y *= exp(aPt.z);

			aPt.x /= sPt.x;
			aPt.y /= sPt.y;
			aPt.z /= sPt.z;

			bPt.x /= sPt.x;
			bPt.y /= sPt.y;
			bPt.z /= sPt.z;
			///ӳ�䵽��Ӧ�߶ȵ�ͼƬ������/////////sPtΪscale//����һ��
			////////////////////////////////////////////

			double w = (weightsV[i])*std::exp(-((aPt - bPt).dot(aPt - bPt)) / 2) / std::sqrt(sPt.dot(Point3d(1, 1, 1)));
			//���¼����Ȩ�أ�ԭȨ��weightsV[i]Ϊ����SVM�ĵ÷�
			resPoint += w*aPt;//�������ĵ�ľ������Ӧ��Ȩֵ,����Խ��,ȨֵԽ��

			ratPoint.x += w / sPt.x;//��߳���Ȩ��ֵ��ʹ�÷������ͼ��Ȩ�ر�С
			ratPoint.y += w / sPt.y;
			ratPoint.z += w / sPt.z;

		}
		resPoint.x /= ratPoint.x;
		resPoint.y /= ratPoint.y;
		resPoint.z /= ratPoint.z;
		return resPoint;//���ر���Χ��Ӱ����ֵ
	}

	double getResultWeight(const Point3d& inPt) const
	{
		double sumW = 0;//���շ��ص�ֵ
		int num = 0;
		size_t aa = positionsV.size();//λ�õ�ĸ���
		int len = int(aa);//λ�õ�ĸ���
		for (size_t i = 0; i < aa; i++)
		{
			Point3d aPt = positionsV[i];
			Point3d sPt = densityKernel;

			sPt.x *= exp(aPt.z);
			sPt.y *= exp(aPt.z);

			aPt -= inPt;//��������֮��

			aPt.x /= sPt.x;
			aPt.y /= sPt.y;
			aPt.z /= sPt.z;
			/*-----------------��黹�����Ż������ԵĿ��ǣ�Ȩ�ص�ѡȡ--begin-----------------*/
			if (aa > 10)//�������aa��������Сʱ  
			{
				double mm = aPt.dot(aPt);

				if (aPt.dot(aPt) <= 0.5)//�������ŷʽ����
				{

					if (weightsV[i] > 0.6)//weightsV[i] Ϊsvm��Ȩֵ
					{
						sumW += 0.35;
					}
					else if (weightsV[i] > 0.3)
					{
						sumW += 0.3;
					}
					continue;
				}
				//////////////////////////ԭʼ///////////
				sumW += (weightsV[i])*std::exp(-(aPt.dot(aPt)) / 2) / std::sqrt(sPt.dot(Point3d(1, 1, 1)));
				/////////////////////////////////////////
			}
			else
			{
				//sumW+=weightsV[i];
				sumW += (weightsV[i])*std::exp(-(aPt.dot(aPt)) / 2)*2.8;

			}
			/*---------���Ż�����-----------------*/
			/*------��黹�����Ż������ԵĿ��ǣ�Ȩ�ص�ѡȡ--end------*/

			return sumW;//��������Ȩֵ
		}

		Point3d moveToMode(Point3d aPt, Rect inR, const vector<Rect>list) const
		{
			//��ֵƯ�ƺ��λ��
			Point3d bPt;
			for (int i = 0; i < iterMax; i++)
			{
				bPt = aPt;
				aPt = getNewValue(bPt, inR, list);
				if (getDistance(aPt, bPt) <= modeEps)//С����ֵʱ���أ�˵���ﵽ�ȶ�״̬
				{
					break;
				}
			}
			return aPt;//�����ȶ�״̬��ֵ
		}

		double getDistance(Point3d p1, Point3d p2) const
		{//����ŷʽ����
			Point3d ns = densityKernel;
			ns.x *= exp(p2.z);
			ns.y *= exp(p2.z);
			p2 -= p1;
			p2.x /= ns.x;
			p2.y /= ns.y;
			p2.z /= ns.z;
			return p2.dot(p2);
		}
	};

	//��groupRectangles_meanshift1����
	void groupRectangles_meanshift2(vector<Rect>& rectList, double detectThreshold, vector<double>* foundWeights,
		vector<double>& scales, Size winDetSize)
	{
		int detectionCount = (int)rectList.size();//���εĸ���
		vector<Point3d> hits(detectionCount), resultHits;
		vector<double> hitWeights(detectionCount), resultWeights;
		Point2d hitCenter;

		for (int i = 0; i < detectionCount; i++)
		{
			hitWeights[i] = (*foundWeights)[i];
			hitCenter = (rectList[i].tl() + rectList[i].br())*(0.5); //center of rectangles
			hits[i] = Point3d(hitCenter.x, hitCenter.y, std::log(scales[i]));//��������x��y��log(scale)
		}


		if (foundWeights)
			foundWeights->clear();

		double logZ = std::log(1.3);
		Point3d smothing(8, 16, logZ);

		MeanshiftGrouping msGrouping(smothing, hits, rectList, hitWeights, 1e-5, 100);//�õ�
		///////////////////////////////////////////////////////////////////////
		msGrouping.getModes(resultHits, resultWeights, 1);
		//////////////////////////////////////////////////////////////////////

		rectList.clear();
		for (unsigned i = 0; i < resultHits.size(); ++i)
		{

			double scale = exp(resultHits[i].z);//��ԭ�߶�
			hitCenter.x = resultHits[i].x;//��������
			hitCenter.y = resultHits[i].y;
			Size s(int(winDetSize.width*scale), int(winDetSize.height*scale));//��ԭ���Ĵ�С
			Rect resultRect(int(hitCenter.x - s.width / 2), int(hitCenter.y - s.height / 2),
				int(s.width), int(s.height));//��ԭ����λ��

			if (resultWeights[i] > detectThreshold)//detectThreshold ������ֵ��Ȩ��ֵ���
			{//���ؾ��ο��Ȩֵ
				rectList.push_back(resultRect);
				foundWeights->push_back(resultWeights[i]);
			}
		}
	}

	void groupRectangles_meanshift1(vector<Rect>& rectList, vector<double>& foundWeights,
		vector<double>& foundScales, double detectThreshold, Size winDetSize)
	{
		groupRectangles_meanshift2(rectList, detectThreshold, &foundWeights, foundScales, winDetSize);
		//rectList�����б�, detectThreshold��ֵ, foundWeights�÷�, foundScales�߶�, winDetSize���ڴ�С
	}
}



int main(int argc,char*argv[])
{

	vector<Rect> found_filtered;

	for ()
	{
		/*----------------------------------�����ж���begin--------------------------------------------------*/
		//////////////������Ȥ�����һ�������HOG����--begin////////////////////
		cvSetImageROI(imageOrigin, r);
		IplImage *test = cvCreateImage(cvGetSize(imageOrigin), 8, 3);
		cvCopyImage(imageOrigin, test);
		cvResetImageROI(imageOrigin);
		IplImage* testTempImg = cvCreateImage(cvSize(40, 40), 8, 3);
		cvZero(testTempImg);
		cvResize(test, testTempImg);
		hog->compute(testTempImg, descriptors, Size(1, 1), Size(0, 0)); //Hog��������
		Mat another(descriptors);
		descriptors.clear();
		////////////������Ȥ�����һ�������HOG����--end///////////////
		
		double ret = 0;
		ret = ssvm.get_calc_liner(vec_count, var_count, a, another, results, alpha, 0) - rho; //����SVM����		
		////////////////////////////
		cvReleaseImage(&test);
		cvReleaseImage(&testTempImg);
		////////////////////////////
		if (ret < 0){
			continue;  //ȥ��SVM ��ֵС��0��ͼ��
		}
	
		rc_.push_back(rcc);
		weights.push_back(ret);
		double rate = max(rcc.width*1.0 / 40, rcc.height*1.0 / 40);
		//ѡȡ�������ֵ����Ϊ�߶ȣ����ǻ������ܵ�������ı䣬Meanshift ȫ����Ҫ��
		//���ﻹ���ԸĽ�
		foundScales.push_back(rate);
		/*----------------------------------�����ж���end--------------------------------------------------*/
	}
	//////////����Ӧ����һ��ѭ������������//////////////




	/////////////////////////�ںϹ���-begin////////////////////////////
	//vector<Rect> rc_;			   //���ο�
	//	vector<double> weights;    //Ȩ�أ�score
	//	vector<double> foundScales;//�����߶� 
	groupRectangles_meanshift1(rc_, weights, foundScales, 0.3, Size(40, 40)); //������ľ��ο�����ں�
	//groupRectangles_meanshift1�ú����������//
	///////////////////////�ںϹ���-end///////////////////////////


	//////////////////�غ�ȥ�ص�һ���������ںϺ�ķ�ֵ//////////////////
	for (int i = 0; i < rc_.size(); i++)
	{
		Rect r = rc_[i];
		found_filtered.push_back(r);//������ο�
	}
	vector<float> found_score(found_filtered.size()); //���ο�ķ���
	for (int i = 0; i < found_filtered.size(); i++)
	{
		Rect r = found_filtered[i];
		cvSetImageROI(imageOrigin, r);
		IplImage *test = cvCreateImage(cvGetSize(imageOrigin), 8, 3);
		cvCopyImage(imageOrigin, test);
		cvResetImageROI(imageOrigin);
		IplImage* testTempImg = cvCreateImage(cvSize(40, 40), 8, 3);
		cvZero(testTempImg);
		cvResize(test, testTempImg);
		hog->compute(testTempImg, descriptors, Size(1, 1), Size(0, 0)); //Hog��������
		Mat another(descriptors);
		descriptors.clear();
		double ret = 0;
		ret = ssvm.get_calc_liner(vec_count, var_count, a, another, results, alpha, 0) - rho;
		cvReleaseImage(&test);
		cvReleaseImage(&testTempImg);
		found_score[i] = ret;
		////////////////////////////
	}
	////////�غ�ȥ�صڶ��������ο��ڵĲ��֣�ȡ��ֵ����/////
	//////////////////////found_filteredΪ�ںϺ�ľ��ο�//////////////////////////
	for (int i = 0; i < found_filtered.size(); i++)
	{
		//�������غ�ȥ����ѡȡ������ֵ�����ο�
		Rect r = found_filtered[i];
		for (int j = 0; j < rc_.size(); j++)

			if (j != i && (r & found_filtered[j]).area() == r.area())
			{//�����ǽ��ص��Ĳ��֣���ֵС�ľ��ο��Ȩ����Ϊ-1,Ϊ��ȡ���ֵ
			if (found_score[i]>found_score[j])
			{
				found_score[j] = -1;
				break;
			}
			else
			{
				found_score[i] = -1;
				break;
			}
			}


	}
	////////�غ�ȥ�ص��������ص����֣�ȡ��ֵ����/////
	//////////////////////////////////////////////////////////////////
	for (int i = 0; i < found_filtered.size(); i++)
	{
		//�������غ�ȥ��
		Rect r = found_filtered[i];
		for (int j = 0; j < rc_.size(); j++)
			//�ж��غ��Ƿ������������70%����������д����ԣ�,�ж���Ϊ��һ�����˺�Ľ�������ܴ��ڲ�����ϣ��������������
			if (j != i && (r & found_filtered[j]).area() >= r.area()*0.7 &&found_score[j] != -1 && found_score[i] != -1)
			{//�����ǽ��ص��Ĳ��֣���ֵС�ľ��ο��Ȩ����Ϊ-1
				if (found_score[i]>found_score[j])
				{
					found_score[j] = -1;
					break;
				}
				else{
					found_score[i] = -1;
					break;
				}
			}


	}
	for (int i = 0; i < found_filtered.size(); i++)
	{
		if (found_score[i] == -1)//����ֵ����-1�Ĺ��˵�
		{
			continue;
		}
		Rect r = found_filtered[i];
		r.x -= cvRound(r.width*0.05);
		r.width = cvRound(r.width*1.05);
		r.y -= cvRound(r.height*0.05);
		r.height = cvRound(r.height*1.05);
		rectangle(img_dst, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);	//��ͼ���ϻ����ο�
	}

	return 0;
}

