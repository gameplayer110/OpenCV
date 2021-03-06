// look_up_fps.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <cv.h>  
#include <highgui.h>  

int read_video_info(int para_num, char* paras[])
{
	IplImage *pFrame = NULL;
	CvCapture* pCapture = NULL;

  //创建窗口
  cvNamedWindow("video", 1);


  //参数验证
  if( para_num > 2 ){
      fprintf(stderr, "Usage: 程序名称 [video_file_name]\n");
      return -1;
    }

  //打开摄像头
  if (para_num ==1)
    if( !(pCapture = cvCaptureFromCAM(-1))){
	    fprintf(stderr, "Can not open camera.\n");
        return -2;
      }

  //读取视频文件
  if(para_num == 2)
    if( !(pCapture = cvCaptureFromFile(paras[1]))){
        fprintf(stderr, "Can not open video file %s\n", paras[1]);
        return -2;
      }
  //逐帧读取视频
  while(pFrame = cvQueryFrame( pCapture ))
	{
	  cvShowImage("video",pFrame);

	  char c=cvWaitKey(33);  
	  if (c == 27) break; //当输入ESC键时，退出窗口； 
  
    }
  cvDestroyWindow("video");
  cvReleaseImage(&pFrame);

  
   //获取视频文件信息
	int frameH    = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT);  
	int frameW    = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH);  
	int fps       = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS);  
	int numFrames = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);  
	printf("vedio's width = %d\t height = %d\n video's fps = %d\t numFrames = %d", frameW, frameH, fps, numFrames);

	getchar();
	return 0;
}
