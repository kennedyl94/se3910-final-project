#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
#include "VideoCamera.cpp"
using namespace std;
using namespace cv;

int main()
{
    
  VideoCamera camera;
  camera.CaptureFrame();
  //VideoReceiver receiver();

  while(1)
  {
    //var frame = camera.CaptureFrame();
    //receiver.AcceptFrame( frame );
  }

  return 0;
}