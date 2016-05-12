#include "AudioInterface.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

class AudioReceiver
{
public:
    void receiveAudio();
    AudioReceiver(char* audio);

private:
    AudioInterface *ai;
    int rc;
    char *buffer;
    int bufferSize;
};

AudioReceiver::AudioReceiver(char* audio)
{
    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_PLAYBACK);
    ai->open();
    bufferSize = ai->getRequiredBufferSize();

    buffer = (char*)malloc(bufferSize);

    pthread_create threads[1];//only 1 thread
    int t;
    t = pthread_create(&threads[0], NULL, &receiveAudio, NULL);

    if(t){
        cout << "unable to create thread in AudioReceiver" << endl;
        exit(-1);
    }
}

void *AudioReceiver::receiveAudio()
{
    // Open the file that is going to be read.
    int filedesc = open("audioCapture.bin", O_RDONLY);
    rc = 1;

    do {
        // Read from the file.
        rc = read(filedesc, buffer, bufferSize);

        if (rc > 0) {
            ai->write(buffer, rc);
        }

        //TODO play audio

    } while (rc > 0);
}
