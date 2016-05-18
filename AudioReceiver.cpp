#include "AudioInterface.h"
#include "AudioReceiver.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

AudioReceiver::AudioReceiver(char* audio, char* addr)
{
    int portno, n;
       struct sockaddr_in serv_addr;
       struct hostent *server;

       char buffer[256];
       if (argc < 3) {
          fprintf(stderr,"usage %s hostname port\n", argv[0]);
          exit(0);
       }

       // Read the port number from the command line.
       portno = atoi(argv[2]);

       // Open a socket. See man socket for details.
       sockfd = socket(AF_INET, SOCK_STREAM, 0);

       // If the socket is less than 0, there is an error opening the socket.
       // Otherwise, the socket has been opened.
       if (sockfd < 0)
       {
           error("ERROR opening socket");
       }

       // Get the hostname.
       server = gethostbyname(addr);

       // If the server is NULL, there is no such host.
       if (server == NULL) {
           fprintf(stderr,"ERROR, no such host\n");
           exit(0);
       }

       // Set the values in the buffer to zero.
       memset((void*)&serv_addr, 0, sizeof(serv_addr));

       // Set up the server address type.
       serv_addr.sin_family = AF_INET;

       // Copy the data.
       memcpy((void*)&serv_addr.sin_addr.s_addr,(void*)server->h_addr, server->h_length);

       // Convert the byte ordering from the host byte ordering to the network byte ordering.
       serv_addr.sin_port = htons(portno);

       // connect to the socket.
       if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
       {
           error("ERROR connecting");
       } //end socket stuff


    cout << "AudioReceiver created" << endl;
    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_PLAYBACK);
    ai->open();
    bufferSize = ai->getRequiredBufferSize();

    buffer = (char*)malloc(bufferSize);

    pthread_t threads[1];//only 1 thread
    int t;
    t = pthread_create(&threads[0], NULL, &tmp, this);

    if(t){
        cout << "unable to create thread in AudioReceiver" << endl;
        exit(-1);
    }
	
	//keeps main thread running so other thread can run
	while(true);
}

void* tmp(void* a)
{
	AudioReceiver* b = static_cast<AudioReceiver*>(a);
	b->receiveAudio();
}

void AudioReceiver::receiveAudio()
{
    cout << "receiving audio" << endl;
    // Open the file that is going to be read.
	//TODO replace file read with network buffer read
//	int filedesc = open("clicks.wav", O_RDONLY);
    rc = 1;

    do {
        // Read from the file.
        rc = read(sockfd, buffer, bufferSize);

        if (rc > 0) {
            ai->write(buffer, rc);
        }

    } while (rc > -1);
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        exit(0);
    }

    AudioReceiver ar(argv[1]);
    return 0;
}
