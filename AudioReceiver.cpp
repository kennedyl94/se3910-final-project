#include "AudioInterface.h"
#include "AudioReceiver.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

AudioReceiver::AudioReceiver(char* audio)
{
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
	int filedesc = open("clicks.wav", O_RDONLY);
    rc = 1;

    do {
        // Read from the file.
        rc = read(filedesc, buffer, bufferSize);

        if (rc > 0) {
            ai->write(buffer, rc);
        }

    } while (rc > 0);
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        exit(0);
    }

    AudioReceiver ar(argv[1]);
    return 0;
}
