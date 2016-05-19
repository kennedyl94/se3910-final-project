#include<iostream>
#include<opencv2/opencv.hpp>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <thread>

#include "video.h"
#include "videodisplay.h"
#include "videocamera.h"

VideoCamera::VideoCamera( ) 
{
  cout << "VideoCamera connecting to camera..." << endl;

  camera = new VideoCapture( 0 );
  camera -> set( CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH );
  camera -> set( CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT );
    
  if ( !camera -> isOpened() ) 
  {
    cout << "VideoCamera failed to connect to the camera." << endl;
    exit ( -1 );
  }
    
  cout << "VideoCamera connected to camera." << endl;
}

void VideoCamera::capture( )
{
  cout << "Frame capturing..." << endl;
  
  camera -> retrieve( frame, 0 );
  
  cout << "Frame captured." << endl;
}

void VideoCamera::start( ) 
{
  cout << "VideoCamera starting thread..." << endl;
  
  running = true;
  thread = std::thread( &VideoCamera::run, this );
  
  cout << "VideoCamera started thread." << endl;
}

void VideoCamera::run( ) 
{
  cout << "VideoCamera starting capture loop..." << endl;
  
  // QPixmap BLUE( "blue.png" );
  // QPixmap GREEN( "green.png" );
  // bool blue = true;
  
  while ( running ) 
  {
    capture( );
    //display -> showFrame();
    //TODO: send video
    
    // if ( blue == true )
    // {
      // display -> showFrame( BLUE );
      // blue = false;
    // }
    // else
    // {
      // display -> showFrame( GREEN );
      // blue = true;
    // }
  }
  
  cout << "VideoCamera ended capture loop." << endl;
}

void VideoCamera::stop( ) 
{
  cout << "VideoCamera stopping thread..." << endl;
    
  running = false;
  thread.join( );
    
  cout << "VideoCamera stopped thread." << endl;
}

VideoCamera::~VideoCamera( ) 
{
  cout << "VideoCamera destructing..." << endl;
   
  delete camera;
   
  cout << "VideoCamera destructed." << endl;
}
