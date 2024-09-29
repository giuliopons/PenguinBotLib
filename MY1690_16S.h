#ifndef MY1690_16S_H
#define MY1690_16S_H

#include <Arduino.h>
#include "NeoSWSerial.h"

// Pin non utilizzato:
// #define MY1690_PIN 8

// Pin per accendere l'amplificatore
#define HT6871_PIN 7

class MY1690_16S {

public:
    MY1690_16S(NeoSWSerial& serial);

    
    String playStatus[5] = {"0", "1", "2", "3", "4"}; // STOP PLAYING PAUSE FF FR

    void playSong(unsigned char num);
    String getPlayStatus();
    String getStatus();
    void stopPlay();
    void setVolume(unsigned char vol);
    void volumePlus();
    void volumeDown();
    void setPlayMode(unsigned char mode);
    void ampMode(int p, bool m);
    void init( unsigned char vol = 255);

private:
    byte volume = 15;
    
    void checkCode(unsigned char* vs);

    NeoSWSerial& mp3Serial;
    byte CMD_MusicPlay[5] = {0x7E, 0x03, 0x11, 0x12, 0xEF};
    byte CMD_MusicStop[5] = {0x7E, 0x03, 0x1E, 0x1D, 0xEF};
    byte CMD_MusicNext[5] = {0x7E, 0x03, 0x13, 0x10, 0xEF};
    byte CMD_MusicPrev[5] = {0x7E, 0x03, 0x14, 0x17, 0xEF};
    byte CMD_VolumePlus[5] = {0x7E, 0x03, 0x15, 0x16, 0xEF};
    byte CMD_VolumeDown[5] = {0x7E, 0x03, 0x16, 0x15, 0xEF};
    byte CMD_VolumeSet[6] = {0x7E, 0x04, 0x31, 0x00, 0x00, 0xEF};
    byte CMD_PlayMode[6] = {0x7E, 0x04, 0x33, 0x00, 0x00, 0xEF};
    byte CMD_SongSelet[7] = {0x7E, 0x05, 0x41, 0x00, 0x00, 0x00, 0xEF};
    byte CMD_getPlayStatus[5] = {0x7E, 0x03, 0x20, 0x23, 0xEF};
};

#endif // MY1690_16S_H
