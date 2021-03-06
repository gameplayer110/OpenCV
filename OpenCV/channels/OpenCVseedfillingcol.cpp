/************************************************************************
* Copyright(c) 2014 tuling56
*
* File:	OpenCVseedfillingcol.cpp
* Brief: 采用种子填充实现彩色图像分割，在处理库opencv下，采用改进的种子算法，
         实现对彩色图像的分割,程序中只包括代码部分
* Source:http://www.pudn.com/downloads372/sourcecode/graph/opencv/detail1609603.html
* Status:  添加了绘制被重绘区域的外接矩形的代码，轮廓绘制还存在着问题---->2015年1月21日
* Date:	[11/30/2014 jmy]
************************************************************************/
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;

IplImage* color_img0;
IplImage* color_img;
IplImage* mask;
IplImage* gray_img0 = NULL;
IplImage* gray_img = NULL;

int ffill_case = 1; //控制漫水的时候是否考虑相邻的种子点，共有三种模式
int lo_diff = 20, up_diff = 20;
int connectivity = 4; //连通性，共有两种，4连通和8连通
int is_color = 1;
int is_mask = 0;
int new_mask_val = 255;

static void on_mouse( int event, int x, int y, int flags,void* )
{
  if( !color_img )   return;
  switch( event )
  {
  case CV_EVENT_LBUTTONDOWN:
    {
        CvPoint seed = cvPoint(x,y);  //获得种子点
        int lo = ffill_case == 0 ? 0 : lo_diff;//若是简单模式，则直接置上下容差为0
        int up = ffill_case == 0 ? 0 : up_diff;
        //填充的模式是在这里控制的
		int flags = connectivity + (new_mask_val << 8) +(ffill_case == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
        int b = rand() & 255, g = rand() & 255, r = rand() & 255; //给种子点随机分配颜色
		
		CvConnectedComp comp;
		if( is_mask )
          cvThreshold( mask, mask, 1, 128, CV_THRESH_BINARY );
       
		if( is_color ){
          CvScalar color = CV_RGB( r, g, b );//新颜色
          cvFloodFill( color_img, seed, color, CV_RGB( lo, lo, lo ),CV_RGB( up, up, up ), &comp, flags, is_mask ? mask : NULL );
		  //求被重绘区域的外接矩形
		  CvPoint tl = cvPoint(comp.rect.x, comp.rect.y);
		  CvPoint br = cvPoint(comp.rect.x + comp.rect.width, comp.rect.y + comp.rect.height);
		  cvRectangle(color_img, tl, br, CV_RGB(0, 255, 0), 2);
		  //求被重绘区域的轮廓
		  //cvDrawContours(color_img, comp.contour, cvScalar(255, 0, 0), cvScalar(0, 255, 255), 3, 3);//为什么没有数据
		  cvShowImage( "image", color_img );
        }
        else
        {
          CvScalar brightness = cvRealScalar((r*2 + g*7 + b + 5)/10);
          cvFloodFill( gray_img, seed, brightness, cvRealScalar(lo),cvRealScalar(up), &comp, flags, is_mask ? mask : NULL );
		  CvPoint tl = cvPoint(comp.rect.x, comp.rect.y);
		  CvPoint br = cvPoint(comp.rect.x + comp.rect.width, comp.rect.y + comp.rect.height);
		  cvRectangle(gray_img, tl, br, cvScalarAll(255), 1);
		  cvShowImage( "image", gray_img );
        }
        printf("%g pixels were repainted\n", comp.area );


        if( is_mask )
          cvShowImage( "mask", mask );
    }
    break;
  }
}

int OpenCVseedfillingcol( int argc, char** argv )
//int main(int argc, char** argv)
{
  char* filename = argc >= 2 ? argv[1] : (char*)"samples/200.png";
  if( (color_img0 = cvLoadImage(filename,1)) == 0 )   return 0;

  printf( "Hot keys: \n"
        "\tESC - quit the program\n"
        "\tc - switch color/grayscale mode\n"
        "\tm - switch mask mode\n"
        "\tr - restore the original image\n"
        "\ts - use null-range floodfill\n"
        "\tf - use gradient floodfill with fixed(absolute) range\n"
        "\tg - use gradient floodfill with floating(relative) range\n"
        "\t4 - use 4-connectivity mode\n"
        "\t8 - use 8-connectivity mode\n" );
    
  color_img = cvCloneImage( color_img0 ); //原始彩色图像备份一下
  gray_img0 = cvCreateImage( cvSize(color_img->width, color_img->height), 8, 1 );
  cvCvtColor( color_img, gray_img0, CV_BGR2GRAY );
  gray_img = cvCloneImage( gray_img0 ); //生成的灰度图备份
  mask = cvCreateImage( cvSize(color_img->width + 2, color_img->height + 2), 8, 1 );//掩码在宽和高上都延伸了2

  cvNamedWindow( "image", 0 );
  cvCreateTrackbar( "lo_diff", "image", &lo_diff, 255, NULL );
  cvCreateTrackbar( "up_diff", "image", &up_diff, 255, NULL );

  setMouseCallback( "image",on_mouse,0);

  for(;;)
  {
	//彩色和灰度切换
    if( is_color )
        cvShowImage( "image", color_img );
    else
        cvShowImage( "image", gray_img );

	//键盘操作的模式控制
    int c = cvWaitKey(0);
    switch( c )
    {
    case '\x1b':
        printf("Exiting ...\n");
        goto exit_main;
    case 'c':
        if( is_color ) { //原来是彩色空间，切换为灰度
          printf("Grayscale mode is set\n");
          cvCvtColor( color_img, gray_img, CV_BGR2GRAY );
          is_color = 0;
        }
        else
        {
          printf("Color mode is set\n");
          cvCopy( color_img0, color_img, NULL );
          cvZero( mask );
          is_color = 1;
        }
        break;
    case 'm':  
        if( is_mask ) { //如果原来用掩码，则新的不适用掩码
          cvDestroyWindow( "mask" );
          is_mask = 0;
        }
        else
        {
          cvNamedWindow( "mask", 0 );
          cvZero( mask );
          cvShowImage( "mask", mask );
          is_mask = 1;
        }
        break;
    case 'r':
        printf("Original image is restored\n");
        cvCopy( color_img0, color_img, NULL );
        cvCopy( gray_img0, gray_img, NULL );
        cvZero( mask );
        break;
    case 's':
        printf("Simple floodfill mode is set\n");
        ffill_case = 0;
        break;
    case 'f':
        printf("Fixed Range floodfill mode is set\n");
        ffill_case = 1;
        break;
    case 'g':
        printf("Gradient (floating range) floodfill mode is set\n");
        ffill_case = 2;
        break;
    case '4':
        printf("4-connectivity mode is set\n");
        connectivity = 4;
        break;
    case '8':
        printf("8-connectivity mode is set\n");
        connectivity = 8;
        break;
    }
  }

exit_main: //竟然用标号

  cvDestroyWindow( "test" );
  cvReleaseImage( &gray_img );
  cvReleaseImage( &gray_img0 );
  cvReleaseImage( &color_img );
  cvReleaseImage( &color_img0 );
  cvReleaseImage( &mask );

  return 1;
}


//功能测试区
//int main()
//{
//	OpenCVseedfillingcol(0,NULL);
//	return 0;
//}