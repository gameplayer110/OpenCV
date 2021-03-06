///**
// * @file Sobel & Laplace Demo
// * @brief Sample code using Sobel and/orScharr and Laplace OpenCV functions to make a simple Edge Detector
// * @author Wei Lan
// */
//
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int soble_laplace( )
{

  Mat src, src_gray;
  Mat grad; 
  char* window_name1 = "Sobel Demo";
  char* window_name2 = "Laplace Demo ";

  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  int kernel_size = 3; 

  /// Load an image
  src = imread("../happycat.png");

  if( !src.data )
    { return -1; }

  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
  cvtColor( src, src_gray, CV_RGB2GRAY );


  /////////////////////////// Sobe l////////////////////////////////////
  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  //Calculates the first, second, third, or mixed image derivatives using an extended Sobel operator.
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );   
  convertScaleAbs( grad_x, abs_grad_x );
  /// Gradient Y  
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );   
  convertScaleAbs( grad_y, abs_grad_y );
  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  namedWindow( window_name1, CV_WINDOW_AUTOSIZE );
  imshow( window_name1, grad );
  imwrite("Sobel.png",grad);

  /////////////////////////////////// Laplace ///////////////////////////////
  Mat abs_dst,dst;
  Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( dst, abs_dst );
  namedWindow( window_name2, CV_WINDOW_AUTOSIZE );
  imshow( window_name2, abs_dst );
  imwrite("Laplace.png",grad);

  waitKey(0);
  return 0;
}