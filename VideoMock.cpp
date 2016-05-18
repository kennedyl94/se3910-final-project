#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
#include "videodisplay.h"
#include <unistd.h>
#include "videoreceive.h"
using namespace std;
using namespace cv;



//this is the main!!!


int main( int argc, char **argv )
{
  
  QApplication app(argc, argv);
  if(app.arguments().length() < 2){
      cout<<"enter client or server and an audio device path"<<endl;
  } else if(app.arguments()[0]=="client"){
      VideoReceive rec(app.arguments()[1]);
      rec.show();


  }else if(app.arguments()[0]=="server"){
      VideoDisplay display(app.arguments()[1]);
      display.show();
  }

  return app.exec();

  
}
