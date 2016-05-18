#ifndef VIDEORECEIVE_H
#define VIDEORECEIVE_H
#include <QLabel>
#include "videocamera.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "AudioReceiver.h"

class VideoReceive
{
  //signals:
    // void changeFrame( QPixmap frame );

  private:
    QLabel *imageLabel;

    static QApplication app;

    QTcpSocket sock;
    void readData(int length, char *ret);
    AudioReceiver audio;


    // QPixmap RED;
    // QPixmap BLUE;
    // QPixmap GREEN;

  public:
    VideoReceive(QString path); //constructor
    void changeFrame( );

    static void AppInit();
    static void AppExec();

  private slots:
    void readyRead();

};


#endif // VIDEODISPALY_H
