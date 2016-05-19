#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
#include <unistd.h>

#include "VideoCamera.cpp"

using namespace std;
using namespace cv;

int main( int argc, char **argv )
{
  QApplication app(argc, argv);
  
  VideoDisplay display;
  display.show();

  VideoCamera camera( );

  return app.exec();
}