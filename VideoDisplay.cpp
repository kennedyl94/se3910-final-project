#include "Video.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include <QApplication>
#include <QMainWindow>
#include <QPrinter>
#include <QtGui>

using namespace std;
using namespace cv;

class VideoDisplay
{
  //signals:
    // void changeFrame( QPixmap frame );

  private:
    QLabel *imageLabel;
    
    static QApplication app;
    
    // QPixmap RED;
    // QPixmap BLUE;
    // QPixmap GREEN;
  
  public:
    VideoDisplay( ); //constructor
    void changeFrame( );
    
    static void AppInit( );
    static void AppExec( );
    
};

VideoDisplay::VideoDisplay( )
{
  cout << "VideoDisplay Constructor entered." << endl;

  // create the label that will display the frames
  imageLabel = new QLabel( );
  imageLabel -> setBackgroundRole( QPalette::Base );
  imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
  imageLabel -> setScaledContents( true );
  
  // set the current frame to a red test image
  QPixmap RED( "red.png" );
  imageLabel->setPixmap( RED );
  
  // create a layout
  QVBoxLayout *layout = new QVBoxLayout( );
  layout->addWidget( imageLabel );
  
  // create a widget
  QWidget widget;
  widget.setWindowTitle( "Z.J.L. Image Viewer" );
  widget.resize( 450, 340 );
  widget.setLayout( layout );
  widget.show( );
  
  
  
  cout << "VideoDisplay Constructor exited." << endl;

      // QPixmap RED("red.png");
      // //GREEN = QPixmap("green.png");
      // //BLUE = QPixmap("blue.png");
      // imageLabel->setPixmap( RED );
};

void VideoDisplay::changeFrame( )
{
};

void VideoDisplay::AppInit( )
{
  cout << "VideoDisplay::AppInit entered." << endl;
  
  // create a application
  int zero = 0; // need to specify that it's an int not reference
  // VideoDisplay::app( zero, NULL );
  VideoDisplay::app;
  
  cout << "VideoDisplay::AppInit exited." << endl;
};

void VideoDisplay::AppExec( )
{
  cout << "VideoDisplay::AppExec entered." << endl;
  
  // begin execution of the UI
  VideoDisplay::app.exec( );
  
  cout << "VideoDisplay::AppExec exited." << endl;
};



    // void green()
    // {
      //  //QPixmap BLUE = QPixmap("blue.png");
      //imageLabel->setPixmap( BLUE );
    // }
    
    // static byte * matToBytes( Mat image )
    // {
      // int size = image.total() * image.elemSize();
      // byte * bytes = new byte[size];  // you will have to delete[] that later
      // std::memcpy(bytes,image.data,size * sizeof(byte));
    // }
    
    // static Mat bytesToMat( byte * bytes, int width, int height )
    // {
      // Mat image = Mat(height,width,CV_8UC3,bytes).clone(); // make a copy
      // return image;
    // }
    
   
    // static void InitStart( int argc, char **argv )
    // {
      // int zero = 0;
      // char emptyArray[0] = {}; 
      // app( argc, NULL );
    // }

    // static void InitEnd( )
    // {
      // app.exec();
    // }
      
      
      // QApplication app(argc, argv);
    // QWidget widget;

    // QVBoxLayout *layout = new QVBoxLayout();

	// //we needed an absolute pass, otherwise was failing
    // QString fileName = "/home/debian/Desktop/Lab6/image.png";
    // ImageViewer *imageViewer = new ImageViewer();
    // imageViewer->open(fileName);
    // layout->addWidget(imageViewer);

    // DigitalClock *clock = new DigitalClock();
    // layout->addWidget(clock);

    // button = new QPushButton();
    // button->setText("SPEAK");
    // layout->addWidget(button);

    // widget.connect(button, SIGNAL(released()), clock, SLOT(handleButton()));
    // widget.setLayout(layout);
    // widget.show();
	// return app.exec();
      
      
      
      
      
      
      
      
      
      
      //char array[0] = {}; //TODO
      // QApplication has several overloads, need to specify that an int of 0 is desired not null.
      // int zero = 0;
      
      // QApplication app( zero, NULL );
      // app.exec();
      // //return app;
    // }

    // void DisplayFrame( Mat frame )
    // {
      // Mat dest;
      // cvtColor( frame, dest, CV_BGR2RGB );
      // QImage image( (uchar*)dest.data, dest.cols, dest.rows, QImage::Format_RGB888 );
        
      // if ( image.isNull() ) 
      // {
        // return;
      // }
      
      // imageLabel -> setPixmap( QPixmap::fromImage(image) );
    // };
    
    // void DisplayFrame( QPixmap pixmap )
    // {
      // imageLabel->setPixmap( pixmap );
    // };

