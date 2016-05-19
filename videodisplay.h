#ifndef VIDEODISPALY_H
#define VIDEODISPALY_H
#include <QLabel>
#include "videocamera.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "AudioCapture.h"


class VideoDisplay : public QMainWindow
{

  private:
    QLabel *imageLabel;
    
    QTcpServer serv; //LUCAS
    QTcpSocket* sock; //LUCAS
    AudioCapture audio; //LUCAS

  public:
    explicit VideoDisplay( QWidget *parent = 0 );
     VideoDisplay(QString path); //LUCAS constructor
    ~VideoDisplay( );
  
  public slots:
    void showFrame( QByteArray frame );
    void showFrame( QPixmap frame );
    
  private slots:
    void accepted(); //LUCAS
    void send(); //LUCAS
};

#endif // VIDEODISPALY_H