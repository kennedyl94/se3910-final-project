#ifndef VIDEODISPALY_H
#define VIDEODISPALY_H
#include <QLabel>
#include "videocamera.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class VideoDisplay
{
  //signals:
    // void changeFrame( QPixmap frame );

  private:
    QLabel *imageLabel;

    static QApplication app;
    VideoCamera cam;
    QTcpServer serv;
    QTcpSocket* sock;



    // QPixmap RED;
    // QPixmap BLUE;
    // QPixmap GREEN;

  public:
    VideoDisplay( ); //constructor
    void changeFrame( );

    static void AppInit( );
    static void AppExec( );

  private slots:
    void accepted();
    void send();

};


#endif // VIDEODISPALY_H
