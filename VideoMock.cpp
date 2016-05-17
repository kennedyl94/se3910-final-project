#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
#include "VideoCamera.cpp"
#include <unistd.h>
using namespace std;
using namespace cv;

int main( int argc, char **argv )
{
  cout << "Main loop entered." << endl;
  
  VideoDisplay display(  );
  
  cout << "Main loop exited." << endl;
  
  
  
  
  //int argc, char **argv
  
  //VideoDisplay::Init();
  
  
  //VideoDisplay::InitStart( argc, argv );
  
  
  
  
  
  
  
  //VideoDisplay::InitEnd(  );

  // QString redPath = "red.png";
  // QString greenPath = "green.png";
  // QString bluePath = "blue.png";
  
  // QImage redImage( redPath );
  // QImage greenImage( greenPath );
  // QImage blueImage( bluePath );
  
  // if ( redImage.isNull() )
  // {
    // cout << "RED image was null." << endl;
    // return 1;
  // }

  // if ( greenImage.isNull() )
  // {
    // cout << "GREEN image was null." << endl;
    // return 1;
  // }

  // if ( blueImage.isNull() )
  // {
    // cout << "BLUE image was null." << endl;
    // return 1;
  // }
  
  //QPixmap::fromImage( redImage );
  
  //display.green();
  
  //QPixmap RED( QPixmap::fromImage("red.png") );
  
  // QPixmap RED = QPixmap::fromImage( redImage );
  // QPixmap GREEN = QPixmap::fromImage( greenImage );
  // QPixmap BLUE = QPixmap::fromImage( blueImage );

  
  

  
  
  
  
  //QPixmap::fromImage( redImage );
  
  
  
  
  
  // while ( 1 )
  // {
    // display.DisplayFrame( RED );
    // sleep( 1 );
    
    // display.DisplayFrame( GREEN );
    // sleep( 1 );
    
    // display.DisplayFrame( BLUE );
    // sleep( 1 );
  // }

  //VideoCamera camera;
  //camera.CaptureFrame();
  
  
  //VideoReceiver receiver();

  //while(1)
  //{
    //var frame = camera.CaptureFrame();
    //receiver.AcceptFrame( frame );
  //}

  return 0;
}