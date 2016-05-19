#include "Video.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include <QApplication>
#include <QMainWindow>
#include <QPrinter>
#include <QtGui>

using namespace std;
using namespace cv;

class VideoDisplay : public QMainWindow
{

  private:
    QLabel *imageLabel;
    
  public:
    explicit VideoDisplay( QWidget *parent = 0 );
    ~VideoDisplay( );
  
  public slots:
    void showFrame( QByteArray frame );
    void showFrame( QPixmap frame );
};


VideoDisplay::VideoDisplay( QWidget *parent ) : QMainWindow( parent )//, ui( new Ui::VideoDisplay )
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
