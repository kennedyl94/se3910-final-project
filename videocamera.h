#ifndef VIDEOCAMERA_H
#define VIDEOCAMERA_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <thread>

#include "Video.h"
#include "VideoDisplay.cpp"

using namespace std;
using namespace cv;

class VideoCamera // VideoCapture is the name of the object we use to connect to the camera
{
  private:
    VideoCapture* camera;
    std::thread thread;
    bool running;
    Mat frame;

  public:
    VideoCamera( );
    ~VideoCamera( );
    void capture( );
    void start( );
    void run( );
    void stop( );
};

#endif // VIDEOCAMERA_H