#include "VideoReceive.h"
#include<iostream>
//#include<opencv2/opencv.hpp>
#include <QApplication>
#include <QMainWindow>
//#include <QPrinter>
#include <QtGui>
#include "VideoReceive.h"

using namespace std;
using namespace cv;
//connecting to 4000


void VideoReceive::readData(int length, char* ret)
{
    int i = 0;
    while(i<length) {

        i += sock.readData(ret+i,length-i);
    }

}

VideoReceive::VideoReceive(QString path): serv(this), audio(path.toStdString().c_str())
{
  connect(&sock, SIGNAL(readyRead), this, SLOT(readyRead));
  cout << "VideoReceive Constructor entered." << endl;

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



  cout << "VideoReceive Constructor exited." << endl;

      // QPixmap RED("red.png");
      // //GREEN = QPixmap("green.png");
      // //BLUE = QPixmap("blue.png");
      // imageLabel->setPixmap( RED );
}

void VideoReceive::changeFrame( )
{
}


void VideoReceive::readyRead()
{
    while(sock.bytesAvailable() > 0) {
        int size, rows, cols, step;
        char* buffer;

        readData(4, (char*)&size);
        readData(4, (char*)&rows);
        readData(4, (char*)&cols);
        readData(4, (char*)&step);

        buffer = (char*)malloc(size);
        readData(size, buffer);

        QImage imgIn= QImage((uchar*) buffer, cols, rows, step, QImage::Format_RGB888);

        imageLabel->setPixmap(QPixmap::fromImage(imgIn));
        QTimer::singleShot(0, this, SLOT(send()));// change 0 to limit fps more important here
    }
}

