#include "Video.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include "VideoDisplay.cpp"
using namespace std;
using namespace cv;

// VideoCapture is the name of the object we use to connect to the camera
class VideoCamera
{
  private:
  
      VideoDisplay *display;
      
  public:

    Mat CaptureFrame()
    {
        VideoCapture usbCamera( 0 );
        usbCamera.set( CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH );
        usbCamera.set( CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT );
        if( !usbCamera.isOpened() )
        {
          cout << "Failed to connect to the camera." << endl;
        }
        
        // capture a frame
        Mat frame;
        usbCamera >> frame;
        
        // ensure valid frame
        if( frame.empty() )
        {
          cout << "Failed to capture an image" << endl;
        }
        
        // display frame on the local display
        display -> DisplayFrame( frame );
        
        return frame;
    };

    VideoCamera()
    {
        // create new thread for CaptureFrame
        // pthread_t threads[1];
        // int threadCreateResult;
        // threadCreateResult = pthread_create( &threads[0], NULL, &CaptureFrame, NULL );

        // if( threadCreateResult )
        // {
          // cout << "unable to create thread in Camera" << endl;
          // exit( -1 );
        // }
        
        // initialize the local display
        VideoDisplay display;
    };

};