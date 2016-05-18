#include "AudioInterface.h"
#include "AudioCapture.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)
//port 5000

AudioCapture::AudioCapture(char* audio)
{
    int sockfd, portno;
    socklen_t clilen;

    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int index;



    // Create a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // If the return is less than 0, then the socket failed to create.
    if (sockfd < 0)
    {
       error("ERROR opening socket");
    }

    // Initialize the buffer to all zeros.
    memset((void*) &serv_addr, 0, sizeof(serv_addr));

    // Obtain the port number as an integer.
    portno = 5000;

    // Setup the server address structure.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket appropriately.
    if (bind(sockfd, (struct sockaddr *) &serv_addr,   sizeof(serv_addr)) < 0)
    {
             error("ERROR on binding");
    }
    // Listen on the socket for an incoming connection.  The parameter is the number of connections that can be waiting / queued up.  5 is the maximum allowed by most systems.
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // Block until a client has connected to the server.  This returns a file descriptor for the connection.
   newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
   // If the return is less than 0l, there is an error.
    if (newsockfd < 0)
      {
         error("ERROR on accept");
      }
    // Fill the buffer with all zeros.


    cout << "AudioCapture created" << endl;
    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_CAPTURE);
    ai->open();
    bufferSize = ai->getRequiredBufferSize();

    buffer = (char*)malloc(bufferSize);

    pthread_t threads[1];//only 1 thread
    int t;
	t = pthread_create(&threads[0], NULL, &tmp, this);

    if(t){
        cout << "unable to create thread in AudioCapture" << endl;
        exit(-1);
    }
	
	//keeps main thread running so other thread can run
	while(true);
}

void* tmp(void* a)
{
	AudioCapture* b = static_cast<AudioCapture*>(a);
	b->captureAudio();
}

void AudioCapture::captureAudio()
{
    cout << "capturing audio" << endl;
    // Open the file that is going to be read.
    //TODO replace file write with network buffer write
	int filedesc = open("audioCapture.bin", O_WRONLY | O_CREAT);
    rc = 1;

    do {
        // Fill the buffer with all zeros.
        memset(buffer, 0, bufferSize);

        // Capture from the soundcard
        ai->read(buffer);

        // Write to the file.
        rc = write(filedesc, buffer, bufferSize);

        //TODO send audio
        sendAudio();

    } while (true);
    ai->close();
}

void AudioCapture::sendAudio(){

//    memset(&buffer[0], 0, sizeof(buffer));
    send(newsockfd, buffer, bufferSize, 0);


}

int main(int argc, char* argv[])
{
    if(argc != 2){
        exit(0);
    }

    AudioCapture ac(argv[1]);
    return 0;
}
