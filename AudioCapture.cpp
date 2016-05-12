#include "AudioInterface.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

class AudioCapture
{
public:
    void captureAudio();
    void sendAudio();
    AudioCapture(char* audio);

private:
    AudioInterface *ai;
    int rc;
    char* buffer;
    int bufferSize;
};

AudioCapture::AudioCapture(char* audio)
{
    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_CAPTURE);
    ai->open();
    bufferSize = ai->getRequiredBufferSize();

    buffer = (char*)malloc(bufferSize);

    pthread_create threads[1];//only 1 thread
    int t;
    t = pthread_create(&threads[0], NULL, &captureAudio, NULL);

    if(t){
        cout << "unable to create thread in AudioReceiver" << endl;
        exit(-1);
    }
}

void *AudioCapture::captureAudio()
{
    // Open the file that is going to be read.
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

AudioCapture::sendAudio(){
    //TODO
}
