#include "Video.h"
#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class VideoReceiver
{
  
  private:
    VideoDisplay *display;
  
  public VideoReceiver()
  {
    VideoDisplay display();
  }

  public void AcceptFrame( Mat frame )
  {
    display -> DisplayFrame( frame );
  }

}