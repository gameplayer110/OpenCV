//-----------------------------------��˵����------------------------------
//	����: �Ƚ�Treshold��AdaptiveThreshold��Ч������
//  ���ܣ�cvAdaptiveThreshold()��ʹ��Otsu�㷨����õ�һ��ȫ����ֵ��
//		  Ȼ����������ֵ���ж�ֵ��
//  �ο���http://blog.csdn.net/hitwengqi/article/details/6856768
//-------------------------------------------------------------------------

#include <highgui.h>
#include <math.h>
#include <cv.h>

IplImage *Igray = 0, *It = 0, *Iat;


void threshold_trackbar(int pos)
{
	// תΪ��ֵͼ
	cvThreshold(Igray, It, pos, 255, CV_THRESH_BINARY);
	// ��ʾ��ֵͼ
	cvShowImage("Threshold",It);
}

void threshold_trackbar2(int pos)
{
	// תΪ��ֵͼ
	cvAdaptiveThreshold(Igray, Iat, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 5, (double)pos);
	// ��ʾ��ֵͼ
	cvShowImage("Adaptive Threshold", Iat);
}

//void threshold_trackbar3(int pos)
//{
//	if (pos/2==0){
//		pos = pos + 1;
//	}
//	// תΪ��ֵͼ
//	cvAdaptiveThreshold(Igray, Iat, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, pos, 15);
//	// ��ʾ��ֵͼ
//	cvShowImage("Adaptive Threshold", Iat);
//}


int adaptiveThreshold(char*path)
{
	//IplImage *Igray = 0, *It = 0, *Iat;

	/*if( argc != 7 )
	{
		return -1;
	}*/
	
	//����������
	//double threshold = (double)atof( argv[1] ); //convert string to double
	//int threshold_type = atoi( argv[2] ) ? CV_THRESH_BINARY : CV_THRESH_BINARY_INV;
	//int adaptive_method = atoi( argv[3] ) ? CV_ADAPTIVE_THRESH_MEAN_C : CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	//int block_size = atoi( argv[4] );
	//double offset = (double)atof( argv[5] );

	//�����������Ƕ�cvThreshold()���õ�
	int threshold =  70;
	int threshold_type =CV_THRESH_BINARY;

    //�����������Ƕ�cvAdaptiveThreshold�������õ�
	int adaptive_method = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	int block_size = 3;
	int offset = 15.0;
	
	//���ػҶ�ͼ
	if( ( Igray = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE ) ) == 0 ){
		return -1;
	}
	
	//����ͬ����С8λ�Ҷ�ͼ�������
	It = cvCreateImage( cvSize( Igray -> width, Igray -> height ), IPL_DEPTH_8U, 1 ); //��ͨ��8λ�Ҷ�ͼ
	Iat = cvCreateImage( cvSize( Igray -> width, Igray -> height ), IPL_DEPTH_8U, 1 );
	
	//��ֵ��
	//cvThreshold( Igray, It, threshold, 255, threshold_type );
	//cvAdaptiveThreshold( Igray, Iat, 255, adaptive_method, CV_THRESH_BINARY_INV, block_size, offset );
	
	
	
	//�����������
	cvNamedWindow( "Threshold", 1 );
	cvCreateTrackbar("Threshold", "Threshold", &threshold, 254, threshold_trackbar);
	threshold_trackbar(1);

	cvNamedWindow( "Adaptive Threshold", 1 );
	cvCreateTrackbar("Offset", "Adaptive Threshold", &offset, 60, threshold_trackbar2);
	//cvCreateTrackbar("BlockSize", "Adaptive Threshold", &block_size, 9, threshold_trackbar3);
	threshold_trackbar2(15);
	//threshold_trackbar3(3);
	
	cvNamedWindow("Src", 1);
	cvShowImage( "Src", Igray );
	//cvShowImage( "Threshold", It );
	//cvShowImage( "Adaptive Threshold", Iat );
	cvWaitKey(0);
	
	//�����ڴ�
	cvReleaseImage( &Igray );
	cvReleaseImage( &It );
	cvReleaseImage( &Iat );
	cvDestroyWindow( "Src" );
	cvDestroyWindow( "Threshold" );
	cvDestroyWindow( "Adaptive Threshold" );

	return 0;
}

////���ܲ�����
//int main(int argc, char**argv)
//{
//	adaptiveThreshold("samples\\icdar2011\\train_textloc\\127.jpg");
//
//	return 0;
//}