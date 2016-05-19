#include "videodisplay.h"
#include<iostream>
//#include<opencv2/opencv.hpp>
#include <QApplication>
#include <QMainWindow>
//#include <QPrinter>
#include <QtGui>
#include "videodisplay.h"

using namespace std;
using namespace cv;
//listening on 4000

VideoDisplay::VideoDisplay( QWidget *parent ) : QMainWindow( parent )
{
  cout << "VideoDisplay constructing..." << endl;
  
  QWidget * widget = new QWidget( this );
  QVBoxLayout *layout = new QVBoxLayout( widget );

  imageLabel = new QLabel();
  imageLabel -> setBackgroundRole( QPalette::Base );
  imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
  imageLabel -> setScaledContents( true );
  
  QPixmap RED( "red.png" );
  imageLabel -> setPixmap( RED );

  layout -> addWidget( imageLabel );

  widget -> setLayout( layout );
  setCentralWidget( widget );

  setWindowTitle( tr("Z.J.L. Image Viewer") );
  resize( 450, 340 );
  
  cout << "VideoDisplay constructed." << endl;
}

void VideoDisplay::showFrame( QPixmap frame )
{
  cout << "VideoDisplay showing frame..." << endl;
  
  imageLabel -> setPixmap( frame );
  
  cout << "VideoCamera showed frame." << endl;
}

void VideoDisplay::showFrame( QByteArray frame )
{
  cout << "VideoDisplay showing frame..." << endl;
  
  QPixmap pixmap;
  pixmap.loadFromData( QByteArray::fromBase64( frame ), "PNG" );
  imageLabel -> setPixmap( pixmap );
  
  cout << "VideoCamera showed frame." << endl;
}

VideoDisplay::~VideoDisplay( )
{
  cout << "VideoDisplay destructing..." << endl;
  
  cout << "VideoDisplay destructed." << endl;
}

void VideoDisplay::accepted()
{
    sock=serv.nextPendingConnection();
    QTimer::singleShot(0, this, SLOT(send()));// change 0 to limit fps
}

void VideoDisplay::send()
{
    Mat img = cam.CaptureFrame();
    int size = img.total()*img.elemSize();
    sock->write((char*)&size, 4);
    sock->write((char*)&img.rows, 4);
    sock->write((char*)&img.cols, 4);
    sock->write((char*)&img.step, 4);
    sock->write(img.data, size);
    QImage imgIn= QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    imageLabel->setPixmap(QPixmap::fromImage(imgIn));
    QTimer::singleShot(0, this, SLOT(send()));// change 0 to limit fps more important here
}
