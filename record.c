#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <lame/lame.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 1024
#define OUTPUT_BUFFER_SIZE 8192

typedef struct {
    FILE *file;
    lame_t lame;
    unsigned char outputBuffer[OUTPUT_BUFFER_SIZE];
} UserData;

static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    UserData *data = (UserData *)userData;
    const float *input = (const float *)inputBuffer;
    unsigned char *output = data->outputBuffer;

    int writeResult = lame_encode_buffer_interleaved(data->lame, input, framesPerBuffer, output, OUTPUT_BUFFER_SIZE);
    fwrite(output, writeResult, 1, data->file);

    return paContinue;
}

int main() {
    PaStream *stream;
    PaError err;
    UserData userData;
    char fileName[256];
    
    printf("녹음할 파일 이름을 입력하세요: ");
    scanf("%s", fileName);
    getchar();  // 버퍼 비우기

    userData.file = fopen(fileName, "wb");
    if (!userData.file) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    userData.lame = lame_init();
    lame_set_num_channels(userData.lame, 1);
    lame_set_in_samplerate(userData.lame, SAMPLE_RATE);
    lame_set_VBR(userData.lame, vbr_default);
    lame_init_params(userData.lame);

    err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio 초기화에 실패했습니다.\n");
        return 1;
    }

    err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, &userData);
    if (err != paNoError) {
        printf("오디오 스트림을 열 수 없습니다.\n");
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf("오디오 스트림을 시작할 수 없습니다.\n");
        return 1;
    }

    printf("녹음을 시작합니다. 녹음 중에는 Enter 키를 눌러서 녹음을 중지할 수 있습니다.\n");
    getchar();  // Enter 키 입력 대기

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        printf("오디오 스트림을 중지할 수 없습니다.\n");
        return 1;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        printf("오디오 스트림을 닫을 수 없습니다.\n");
        return 1;
    }

    lame_encode_flush(userData.lame, userData.outputBuffer, OUTPUT_BUFFER_SIZE);
    fwrite(userData.outputBuffer, 1, lame_encode_flush(userData.lame, userData.outputBuffer, OUTPUT_BUFFER_SIZE), userData.file);

    lame_close(userData.lame);
    fclose(userData.file);
    Pa_Terminate();

    printf("녹음이 완료되었습니다.\n");

    return 0;
}
