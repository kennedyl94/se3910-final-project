#include "AudioInterface.h"
#include "AudioCapture.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

AudioCapture::AudioCapture(char* audio)
{
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
    //TODO
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        exit(0);
    }

    AudioCapture ac(argv[1]);
    return 0;
}
