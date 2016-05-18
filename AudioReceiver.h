#ifndef AUDIORECEIVER_H
#define AUDIORECEIVER_H

void* tmp(void* a);

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
    int sockfd;
};

#endif
