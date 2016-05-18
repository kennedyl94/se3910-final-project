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


VideoDisplay::VideoDisplay(QString path): serv(this), audio(path.toStdString().c_str())
{
  connect(&serv, SIGNAL(newConnection()), this, SLOT(accepted()));
  serv.listen(QHostAddress::Any, 4000);
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
}

void VideoDisplay::changeFrame( )
{
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
