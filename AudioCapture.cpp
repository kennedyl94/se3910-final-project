#include "AudioInterface.h"

#define SAMPLING_RATE (22500)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

class AudioCapture
{
public:
    void captureAudio();
    void sendAudio();
    AudioCapture(AudioInterface audio);

private:
    AudioInterface *ai;
    int rc;
    char* buffer;
    int bufferSize;
    int filedesc;
};

AudioCapture::AudioCapture(char* audio)
{
    ai = new AudioInterface(audio, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_CAPTURE);
    ai->open();
    bufferSize = ai->getRequiredBufferSize();

    buffer = (char*)malloc(bufferSize);

    filedesc = open("audioCapture.bin", O_WRONLY | O_CREAT);
    rc = 1;
}

AudioCapture::captureAudio()
{
    do {
        // Fill the buffer with all zeros.
        memset(buffer, 0, bufferSize);

        // Capture from the soundcard
        ai->read(buffer);

        // Write to the file.
        rc = write(filedesc, buffer, bufferSize);

        sendAudio();

    } while (true);
    ai->close();
}

AudioCapture::sendAudio(){
    //TODO
}
