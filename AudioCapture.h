#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

void* tmp(void* a);

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
    int newsockfd;
};

#endif