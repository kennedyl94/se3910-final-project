Linked applications Bitbucket
Teams
Projects
Repositories
Snippets
owner/repository
Find a repository…
 Logged in as krolackz
 
krolackz  SE3910 Final Project
Commits

 krolackz  committed 82f3c93
32 minutes ago
Approve
added files
Participants
 krolackz
Parent commits84cbd1a
Branchesnetwork_and_final
Raw commitView raw commit
Watch Stop watching
Comments (0)


What do you want to say?
Files changed (14)
+135 -0 final/AudioCapture.cpp
+22 -0 final/AudioCapture.h
+135 -0 final/AudioInterface.cpp
+92 -0 final/AudioInterface.h
+126 -0 final/AudioReceiver.cpp
+20 -0 final/AudioReceiver.h
+8 -0 final/Video.h
+52 -0 final/VideoCamera.cpp
+78 -0 final/VideoDisplay.cpp
+34 -0 final/VideoMock.cpp
+86 -0 final/VideoReceive.cpp
+17 -0 final/videocamera.h
+43 -0 final/videodisplay.h
+41 -0 final/videoreceive.h
File final/AudioCapture.cpp
Side-by-side diff  View file   Comment   More
+#include "AudioInterface.h"
+#include "AudioCapture.h"
+#include <iostream>
+#include <cstdlib>
+#include <pthread.h>
+#include <sys/socket.h>
+#include <netinet/in.h>
+#include <arpa/inet.h>
+#include <netdb.h>
+
+using namespace std;
+
+#define SAMPLING_RATE (22500)
+#define NUMBER_OF_CHANNELS (2)
+#define BYTES_PER_SAMPLE (2)
+//port 5000
+
+AudioCapture::AudioCapture(char* audio)
+{
+    int sockfd, portno;
+    socklen_t clilen;
+
+    struct sockaddr_in serv_addr, cli_addr;
+
+    // Create a socket.
+    sockfd = socket(AF_INET, SOCK_STREAM, 0);
+
+    // If the return is less than 0, then the socket failed to create.
+    if (sockfd < 0)
+    {
+       cout << "ERROR opening socket" << endl;
+    }
+
+    // Initialize the buffer to all zeros.
+    memset((void*) &serv_addr, 0, sizeof(serv_addr));
+
+    // Obtain the port number as an integer.
+    portno = 5000;
+
+    // Setup the server address structure.
+    serv_addr.sin_family = AF_INET;
+    serv_addr.sin_addr.s_addr = INADDR_ANY;
+    serv_addr.sin_port = htons(portno);
+
+    // Bind the socket appropriately.
+    if (bind(sockfd, (struct sockaddr *) &serv_addr,   sizeof(serv_addr)) < 0)
+    {
+		cout << "ERROR binding socket" << endl;
+    }
+    // Listen on the socket for an incoming connection.  The parameter is the number of connections that can be waiting / queued up.  5 is the maximum allowed by most systems.
+    listen(sockfd,5);
+    clilen = sizeof(cli_addr);
+
+    // Block until a client has connected to the server.  This returns a file descriptor for the connection.
+    newsockfd = accept(sockfd,
+                (struct sockaddr *) &cli_addr,
+                &clilen);
+    // If the return is less than 0l, there is an error.
+    if (newsockfd < 0)
+      {
+         cout << "ERROR on accept" << endl;
+      }
+    // Fill the buffer with all zeros.
+
+
+    cout << "AudioCapture created" << endl;
+    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_CAPTURE);
+    ai->open();
+    bufferSize = ai->getRequiredBufferSize();
+
+    buffer = (char*)malloc(bufferSize);
+
+    pthread_t threads[1];//only 1 thread
+    int t;
+	t = pthread_create(&threads[0], NULL, &tmp, this);
+
+    if(t){
+        cout << "unable to create thread in AudioCapture" << endl;
+        exit(-1);
+    }
+	
+	//keeps main thread running so other thread can run
+//	while(true);
+}
+
+void* tmp(void* a)
+{
+	AudioCapture* b = static_cast<AudioCapture*>(a);
+	b->captureAudio();
+}
+
+void AudioCapture::captureAudio()
+{
+    cout << "capturing audio" << endl;
+    // Open the file that is going to be read.
+    //TODO replace file write with network buffer write
+	int filedesc = open("audioCapture.bin", O_WRONLY | O_CREAT);
+    rc = 1;
+
+    do {
+        // Fill the buffer with all zeros.
+        memset(buffer, 0, bufferSize);
+
+        // Capture from the soundcard
+        ai->read(buffer);
+
+        // Write to the file.
+        rc = write(filedesc, buffer, bufferSize);
+
+        //TODO send audio
+        sendAudio();
+
+    } while (true);
+    ai->close();
+}
+
+void AudioCapture::sendAudio(){
+
+//    memset(&buffer[0], 0, sizeof(buffer));
+    send(newsockfd, buffer, bufferSize, 0);
+
+
+}
+
+/*
+int main(int argc, char* argv[])
+{
+    if(argc != 2){
+        exit(0);
+    }
+
+    AudioCapture ac(argv[1]);
+    return 0;
+}
+*/
File final/AudioCapture.h
Side-by-side diff  View file   Comment   More
+#ifndef AUDIOCAPTURE_H
+#define AUDIOCAPTURE_H
+
+void* tmp(void* a);
+
+class AudioCapture
+{
+public:
+    void captureAudio();
+    void sendAudio();
+    AudioCapture(char* audio);
+
+
+private:
+    AudioInterface *ai;
+    int rc;
+    char* buffer;
+    int bufferSize;
+    int newsockfd;
+};
+
+#endif
File final/AudioInterface.cpp
Side-by-side diff  View file   Comment   More
+/*
+ * AudioInterface.cpp
+ *
+ *  Created on: Apr 18, 2015
+ *      Author: Walter Schilling
+ *      This is the implementation of the audio Interface.  It will manage the system.
+ */
+
+#include "AudioInterface.h"
+
+/**
+ * This is the constructor for the class.  It will instantiate a new instance of the class.
+ * @param deviceName This is the name of the device.  A typical name might be "plughw:1"
+ * @param samplingRate This is the sampling rate for the audio sampls.  Typical rates are 11000, 22050, and 44100.
+ * @param channels This is the number of channels.  Typically this is 1 or 2.
+ * @param direction This is the direction.  It is either  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
+ */
+AudioInterface::AudioInterface(char* deviceName, unsigned int samplingRate,
+		int channels, snd_pcm_stream_t direction) {
+	// Determine the length of the device name and allocate memory for it.
+	int deviceLength = strlen(deviceName);
+	this->deviceName = (char*) malloc(deviceLength + 1);
+	// Initialize the buffer with the device name.
+	strcpy(this->deviceName, deviceName);
+	this->samplingRate = samplingRate;
+	this->channels = channels;
+	this->direction = direction;
+	this->handle = NULL;
+	this->params = NULL;
+	frames = DEFAULT_FRAME_SIZE;
+}
+
+/**
+ * This is the destructor.  It will clear all allocated space.
+ */
+AudioInterface::~AudioInterface() {
+	free(this->deviceName);
+}
+
+/**
+ * This method will close the given device so it is no longer accessible.
+ */
+
+void AudioInterface::close() {
+	//  close the hardware we are connected to.
+	snd_pcm_drain(handle);
+	snd_pcm_close(handle);
+}
+
+/**
+ * This method will open the given device so that it can be accessed.
+ */
+void AudioInterface::open() {
+	int rc; // This variable is to be used to store the return code from various calls.
+	int dir;
+
+	/* Open PCM device. */
+	snd_pcm_open(&handle, this->deviceName, this->direction, 0);
+
+	/* Allocate a hardware parameters object. */
+	snd_pcm_hw_params_alloca(&params);
+
+	/* Fill it in with default values. */
+	snd_pcm_hw_params_any(handle, params);
+
+	/* Set the desired hardware parameters. */
+
+	/* Interleaved mode */
+	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
+
+	/* Signed 16-bit little-endian format */
+	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
+
+	/* Two channels (stereo) */
+	snd_pcm_hw_params_set_channels(handle, params, this->channels);
+
+	// Set the sampling rate appropriately.
+	snd_pcm_hw_params_set_rate_near(handle, params, &(this->samplingRate),
+			&dir);
+
+	/* Set period size to the given number of frames. */
+	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
+
+	/* Write the parameters to the driver */
+	rc = snd_pcm_hw_params(handle, params);
+	if (rc < 0) {
+		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
+		exit(1);
+	}
+}
+/**
+ * This method will determine, based upon the size of each frame and the number of frames between interrupts the required size of the buffer that is to store the data.
+ * @return
+ */
+int AudioInterface::getRequiredBufferSize() {
+	int dir;
+	int size;
+	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
+	size = frames * this->channels * 2; /* 2 bytes/sample */
+	return size;
+}
+/**
+ * This method will read data from the given sound device into the buffer.
+ * @param buffer This is a pointer to a buffer that is to be written into.  The calee is responsible for providing this buffer.  The buffer must be the size calculated by the getRequiredBufferisze() method.
+ * @return The return will be the number of samples (not bytes) written into the buffer.
+ */
+int AudioInterface::read(char* buffer) {
+	return snd_pcm_readi(handle, buffer, frames);
+}
+
+/**
+ * This method will write data to the soundcard for playback.
+ * @param buffer This is the buffer that is to be written.  It must be of size getRequiredBufferisze().
+ * @param bufferLength This is the number of valid entries in the buffer.
+ */
+void AudioInterface::write(char* buffer, int bufferLength) {
+	int rc; // This variable is to be used to store the return code from various calls.
+	int frames = bufferLength / (2 * this->channels);
+
+	if (bufferLength > 0) {
+		// Write the data to the soundcard.
+		rc = snd_pcm_writei(this->handle, buffer, frames);
+
+		if (rc == -EPIPE) {
+			/* EPIPE means underrun */
+			fprintf(stderr, "underrun occurred\n");
+			snd_pcm_prepare(this->handle);
+		} else if (rc < 0) {
+			fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
+		} else if (rc != (int) frames) {
+			fprintf(stderr, "short write, write %d frames\n", rc);
+		}
+	}
+}
+
File final/AudioInterface.h
Side-by-side diff  View file   Comment   More
+/*
+ * AudioInterface.h
+ *
+ *  Created on: Apr 18, 2015
+ *      Author: Walter Schilling
+ *      This class provides an interface to the audio system that we will be uisng.
+ *      It allows the user to open a device for both capture and playback, and it sets the frame isze accordingly.
+ */
+
+#ifndef AUDIOINTERFACE_H_
+#define AUDIOINTERFACE_H_
+
+#include <alsa/asoundlib.h>
+
+#define DEFAULT_FRAME_SIZE 512
+
+class AudioInterface {
+private:
+	/**
+	 * This is the name of the device.  It is a pointer that will be allocated upon instantiation.
+	 */
+	char* deviceName;
+	/**
+	 * This integer represents the sampling rate for the system.
+	 */
+	unsigned int samplingRate;
+	/**
+	 * This is the number of channels.  2 will be normal for stereo.
+	 */
+	int channels;
+	/**
+	 * This is the direction of the class.  It is either  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
+	 */
+	snd_pcm_stream_t direction;
+	/**
+	 * This is a pointer to a device handle.
+	 */
+	snd_pcm_t *handle; // This is a pointer to the soundcard device.
+
+	/**
+	 * This is a variable which holds the number of frames.
+	 */
+	snd_pcm_uframes_t frames;
+
+	/**
+	 * This is a pointer to the hardware set of parameters.
+	 */
+	snd_pcm_hw_params_t *params;
+
+public:
+	/**
+	 * This is the constructor for the class.  It will instantiate a new instance of the class.
+	 * @param deviceName This is the name of the device.  A typical name might be "plughw:1"
+	 * @param samplingRate This is the sampling rate for the audio sampls.  Typical rates are 11000, 22050, and 44100.
+	 * @param channels This is the number of channels.  Typically this is 1 or 2.
+	 * @param direction This is the direction.  It is either  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
+	 */
+	AudioInterface(char* deviceName, unsigned int samplingRate, int channels, snd_pcm_stream_t direction);
+	/**
+	 * This is the destructor.  It will clear all allocated space.
+	 */
+	virtual ~AudioInterface();
+	/**
+	 * This method will open the given device so that it can be accessed.
+	 */
+	void open();
+
+	/**
+	 * This method will close the given device so it is no longer accessible.
+	 */
+	void close();
+	/**
+	 * This method will read data from the given sound device into the buffer.
+	 * @param buffer This is a pointer to a buffer that is to be written into.  The calee is responsible for providing this buffer.  The buffer must be the isze calculated by the getRequiredBufferisze() method.
+	 * @return The return will be the number of bytes written into the buffer.
+	 */
+	int read(char* buffer);
+
+	/**
+	 * This method will write data to the soundcard for playback.
+	 * @param buffer This is the buffer that is to be written.  It must be of size getRequiredBufferisze().
+	 * @param bufferLength This is the number of valid entries in the buffer.
+	 */
+	void write(char* buffer, int bufferLength);
+	/**
+	 * This method will determine, based upon the size of each frame and the number of frames between interrupts the required size of the buffer that is to store the data.
+	 * @return
+	 */
+	int getRequiredBufferSize();
+};
+
+#endif /* AUDIOINTERFACE_H_ */
File final/AudioReceiver.cpp
Side-by-side diff  View file   Comment   More
+#include "AudioInterface.h"
+#include "AudioReceiver.h"
+#include <iostream>
+#include <cstdlib>
+#include <pthread.h>
+#include <sys/socket.h>
+#include <netinet/in.h>
+#include <arpa/inet.h>
+#include <netdb.h>
+
Add a comment to this line
+using namespace std;
+
+#define SAMPLING_RATE (22500)
+#define NUMBER_OF_CHANNELS (2)
+#define BYTES_PER_SAMPLE (2)
+
+AudioReceiver::AudioReceiver(char* audio, char* addr)
+{
+    int portno, n;
+    struct sockaddr_in serv_addr;
+    struct hostent *server;
+
+    //char* buffer[256];
+    
+	/*if (argc < 3) {
+		fprintf(stderr,"usage %s hostname port\n", argv[0]);
+        exit(0);
+    }*/
+
+    // Hard coded port number for now
+    portno = 5000;
+
+    // Open a socket. See man socket for details.
+    sockfd = socket(AF_INET, SOCK_STREAM, 0);
+
+    // If the socket is less than 0, there is an error opening the socket.
+    // Otherwise, the socket has been opened.
+    if (sockfd < 0)
+    {
+		cout << "ERROR opening socket" << endl;
+    }
+
+    // Get the hostname.
+    server = gethostbyname(addr);
+
+    // If the server is NULL, there is no such host.
+    if (server == NULL) {
+		cout << "no such host" << endl;
+        exit(0);
+    }
+
+    // Set the values in the buffer to zero.
+    memset((void*)&serv_addr, 0, sizeof(serv_addr));
+
+    // Set up the server address type.
+    serv_addr.sin_family = AF_INET;
+
+    // Copy the data.
+    memcpy((void*)&serv_addr.sin_addr.s_addr,(void*)server->h_addr, server->h_length);
+
+    // Convert the byte ordering from the host byte ordering to the network byte ordering.
+    serv_addr.sin_port = htons(portno);
+
+    // connect to the socket.
+    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
+    {
+		cout << "ERROR connecting" << endl;
+    } //end socket stuff
+
+
+    cout << "AudioReceiver created" << endl;
+    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_PLAYBACK);
+    ai->open();
+    bufferSize = ai->getRequiredBufferSize();
+
+    buffer = (char*)malloc(bufferSize);
+
+    pthread_t threads[1];//only 1 thread
+    int t;
+    t = pthread_create(&threads[0], NULL, &tmp, this);
+
+    if(t){
+        cout << "unable to create thread in AudioReceiver" << endl;
+        exit(-1);
+    }
+	
+	//keeps main thread running so other thread can run
+	while(true);
+}
+
+void* tmp(void* a)
+{
+	AudioReceiver* b = static_cast<AudioReceiver*>(a);
+	b->receiveAudio();
+}
+
+void AudioReceiver::receiveAudio()
+{
+    cout << "receiving audio" << endl;
+    // Open the file that is going to be read.
+	//TODO replace file read with network buffer read
+//	int filedesc = open("clicks.wav", O_RDONLY);
+    rc = 1;
+
+    do {
+        // Read from the buffer.
+        rc = read(sockfd, buffer, bufferSize);
+
+        if (rc > 0) {
+            ai->write(buffer, rc);
+        }
+
+    } while (rc > -1);
+}
+
+/*
+int main(int argc, char* argv[])
+{
+    if(argc != 2){
+        exit(0);
+    }
+
+    AudioReceiver ar(argv[1]);
+    return 0;
+}
+*/
File final/AudioReceiver.h
Side-by-side diff  View file   Comment   More
+#ifndef AUDIORECEIVER_H
+#define AUDIORECEIVER_H
+
+void* tmp(void* a);
+
+class AudioReceiver
+{
+public:
+    void receiveAudio();
+    AudioReceiver(char* audio, char* addr);
+
+private:
+    AudioInterface *ai;
+    int rc;
+    char *buffer;
+    int bufferSize;
+    int sockfd;
+};
+
+#endif
File final/Video.h
Side-by-side diff  View file   Comment   More
+
+#ifndef VIDEO_H
+#define VIDEO_H
+
+#define FRAME_WIDTH 640
+#define FRAME_HEIGHT 480
+
+#endif /* VIDEO_H */
File final/VideoCamera.cpp
Side-by-side diff  View file   Comment   More
+#include "Video.h"
+#include<iostream>
+#include<opencv2/opencv.hpp>
+#include "VideoDisplay.cpp"
+using namespace std;
+using namespace cv;
+
+// VideoCapture is the name of the object we use to connect to the camera
+
+Mat VideoCamera::CaptureFrame()
+{
+    VideoCapture usbCamera( 0 );
+    usbCamera.set( CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH );
+    usbCamera.set( CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT );
+    if( !usbCamera.isOpened() )
+    {
+        cout << "Failed to connect to the camera." << endl;
+    }
+
+    // capture a frame
+    Mat frame;
+    usbCamera >> frame;
+
+    // ensure valid frame
+    if( frame.empty() )
+    {
+        cout << "Failed to capture an image" << endl;
+    }
+
+    // display frame on the local display
+    //display -> DisplayFrame( frame );
+
+    return frame;
+}
+
+VideoCamera::VideoCamera()
+{
+    // create new thread for CaptureFrame
+    // pthread_t threads[1];
+    // int threadCreateResult;
+    // threadCreateResult = pthread_create( &threads[0], NULL, &CaptureFrame, NULL );
+
+    // if( threadCreateResult )
+    // {
+    // cout << "unable to create thread in Camera" << endl;
+    // exit( -1 );
+    // }
+
+    // initialize the local display
+    //VideoDisplay::Init();
+    //VideoDisplay display;
+}
File final/VideoDisplay.cpp
Side-by-side diff  View file   Comment   More
+#include "videodisplay.h"
+#include<iostream>
+//#include<opencv2/opencv.hpp>
+#include <QApplication>
+#include <QMainWindow>
+//#include <QPrinter>
+#include <QtGui>
+#include "videodisplay.h"
+
+using namespace std;
+using namespace cv;
+//listening on 4000
+
+
+VideoDisplay::VideoDisplay(QString path): serv(this), audio(path.toStdString().c_str())
+{
+  connect(&serv, SIGNAL(newConnection()), this, SLOT(accepted()));
+  serv.listen(QHostAddress::Any, 4000);
+  cout << "VideoDisplay Constructor entered." << endl;
+
+  // create the label that will display the frames
+  imageLabel = new QLabel( );
+  imageLabel -> setBackgroundRole( QPalette::Base );
+  imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
+  imageLabel -> setScaledContents( true );
+  
+  // set the current frame to a red test image
+  QPixmap RED( "red.png" );
+  imageLabel->setPixmap( RED );
+  
+  // create a layout
+  QVBoxLayout *layout = new QVBoxLayout( );
+  layout->addWidget( imageLabel );
+  
+  // create a widget
+  QWidget widget;
+  widget.setWindowTitle( "Z.J.L. Image Viewer" );
+  widget.resize( 450, 340 );
+  widget.setLayout( layout );
+  widget.show( );
+  
+  
+  
+  cout << "VideoDisplay Constructor exited." << endl;
+
+      // QPixmap RED("red.png");
+      // //GREEN = QPixmap("green.png");
+      // //BLUE = QPixmap("blue.png");
+      // imageLabel->setPixmap( RED );
+}
+
+void VideoDisplay::changeFrame( )
+{
+}
+
+
+void VideoDisplay::accepted()
+{
+    sock=serv.nextPendingConnection();
+    QTimer::singleShot(0, this, SLOT(send()));// change 0 to limit fps
+
+}
+
+void VideoDisplay::send()
+{
+    Mat img = cam.CaptureFrame();
+    int size = img.total()*img.elemSize();
+    sock->write((char*)&size, 4);
+    sock->write((char*)&img.rows, 4);
+    sock->write((char*)&img.cols, 4);
+    sock->write((char*)&img.step, 4);
+    sock->write(img.data, size);
+    QImage imgIn= QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
+
+    imageLabel->setPixmap(QPixmap::fromImage(imgIn));
+    QTimer::singleShot(0, this, SLOT(send()));// change 0 to limit fps more important here
+
+}
File final/VideoMock.cpp
Side-by-side diff  View file   Comment   More
+#include<iostream>
+#include<opencv2/opencv.hpp>
+#include<time.h>
+#include "videodisplay.h"
+#include <unistd.h>
+#include "videoreceive.h"
+using namespace std;
+using namespace cv;
+
+
+
+//this is the main!!!
+
+
+int main( int argc, char **argv )
+{
+  
+  QApplication app(argc, argv);
+  if(app.arguments().length() < 2){
+      cout<<"enter client or server and an audio device path"<<endl;
+  } else if(app.arguments()[0]=="client"){
+      VideoReceive rec(app.arguments()[1]);
+      rec.show();
+
+
+  }else if(app.arguments()[0]=="server"){
+      VideoDisplay display(app.arguments()[1]);
+      display.show();
+  }
+
+  return app.exec();
+
+  
+}
File final/VideoReceive.cpp
Side-by-side diff  View file   Comment   More
+#include "VideoReceive.h"
+#include<iostream>
+//#include<opencv2/opencv.hpp>
+#include <QApplication>
+#include <QMainWindow>
+//#include <QPrinter>
+#include <QtGui>
+#include "VideoReceive.h"
+
+using namespace std;
+using namespace cv;
+//connecting to 4000
+
+
+void VideoReceive::readData(int length, char* ret)
+{
+    int i = 0;
+    while(i<length) {
+
+        i += sock.readData(ret+i,length-i);
+    }
+
+}
+
+VideoReceive::VideoReceive(QString path): serv(this), audio(path.toStdString().c_str())
+{
+  connect(&sock, SIGNAL(readyRead), this, SLOT(readyRead));
+  cout << "VideoReceive Constructor entered." << endl;
+
+  // create the label that will display the frames
+  imageLabel = new QLabel( );
+  imageLabel -> setBackgroundRole( QPalette::Base );
+  imageLabel -> setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
+  imageLabel -> setScaledContents( true );
+
+  // set the current frame to a red test image
+  QPixmap RED( "red.png" );
+  imageLabel->setPixmap( RED );
+
+  // create a layout
+  QVBoxLayout *layout = new QVBoxLayout( );
+  layout->addWidget( imageLabel );
+
+  // create a widget
+  QWidget widget;
+  widget.setWindowTitle( "Z.J.L. Image Viewer" );
+  widget.resize( 450, 340 );
+  widget.setLayout( layout );
+  widget.show( );
+
+
+
+  cout << "VideoReceive Constructor exited." << endl;
+
+      // QPixmap RED("red.png");
+      // //GREEN = QPixmap("green.png");
+      // //BLUE = QPixmap("blue.png");
+      // imageLabel->setPixmap( RED );
+}
+
+void VideoReceive::changeFrame( )
+{
+}
+
+
+void VideoReceive::readyRead()
+{
+    while(sock.bytesAvailable() > 0) {
+        int size, rows, cols, step;
+        char* buffer;
+
+        readData(4, (char*)&size);
+        readData(4, (char*)&rows);
+        readData(4, (char*)&cols);
+        readData(4, (char*)&step);
+
+        buffer = (char*)malloc(size);
+        readData(size, buffer);
+
+        QImage imgIn= QImage((uchar*) buffer, cols, rows, step, QImage::Format_RGB888);
+
+        imageLabel->setPixmap(QPixmap::fromImage(imgIn));
+        QTimer::singleShot(0, this, SLOT(send()));// change 0 to limit fps more important here
+    }
+}
+
File final/videocamera.h
Side-by-side diff  View file   Comment   More
+#ifndef VIDEOCAMERA_H
+#define VIDEOCAMERA_H
+#include<opencv2/opencv.hpp>
+class VideoCamera
+{
+  private:
+
+
+  public:
+
+    Mat CaptureFrame();
+
+    VideoCamera();
+
+};
+
+#endif // VIDEOCAMERA_H
File final/videodisplay.h
Side-by-side diff  View file   Comment   More
+#ifndef VIDEODISPALY_H
+#define VIDEODISPALY_H
+#include <QLabel>
+#include "videocamera.h"
+#include <QtNetwork/QTcpServer>
+#include <QtNetwork/QTcpSocket>
+#include "AudioCapture.h"
+
+class VideoDisplay
+{
+  //signals:
+    // void changeFrame( QPixmap frame );
+
+  private:
+    QLabel *imageLabel;
+
+    static QApplication app;
+    VideoCamera cam;
+    QTcpServer serv;
+    QTcpSocket* sock;
+    AudioCapture audio;
+
+
+
+    // QPixmap RED;
+    // QPixmap BLUE;
+    // QPixmap GREEN;
+
+  public:
+    VideoDisplay(QString path); //constructor
+    void changeFrame( );
+
+    static void AppInit( );
+    static void AppExec( );
+
+  private slots:
+    void accepted();
+    void send();
+
+};
+
+
+#endif // VIDEODISPALY_H
File final/videoreceive.h
Side-by-side diff  View file   Comment   More
+#ifndef VIDEORECEIVE_H
+#define VIDEORECEIVE_H
+#include <QLabel>
+#include "videocamera.h"
+#include <QtNetwork/QTcpServer>
+#include <QtNetwork/QTcpSocket>
+#include "AudioReceiver.h"
+
+class VideoReceive
+{
+  //signals:
+    // void changeFrame( QPixmap frame );
+
+  private:
+    QLabel *imageLabel;
+
+    static QApplication app;
+
+    QTcpSocket sock;
+    void readData(int length, char *ret);
+    AudioReceiver audio;
+
+
+    // QPixmap RED;
+    // QPixmap BLUE;
+    // QPixmap GREEN;
+
+  public:
+    VideoReceive(QString path); //constructor
+    void changeFrame( );
+
+    static void AppInit();
+    static void AppExec();
+
+  private slots:
+    void readyRead();
+
+};
+
+
+#endif // VIDEODISPALY_H
Blog  Support  Plans & pricing  Documentation  API  Site status  Version info  Terms of service  Privacy policy
JIRA  Confluence  Bamboo  SourceTree  HipChat
Atlassian