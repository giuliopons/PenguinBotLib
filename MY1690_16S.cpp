#include "MY1690_16S.h"

MY1690_16S::MY1690_16S(NeoSWSerial& serial) : mp3Serial(serial), volume(15) {}

void MY1690_16S::playSong(unsigned char num, unsigned char vol) {
    setVolume(vol);
    setPlayMode(4);
    CMD_SongSelet[4] = num;
    checkCode(CMD_SongSelet);
    mp3Serial.write(CMD_SongSelet, 7);
    delay(50);
}

String MY1690_16S::getPlayStatus() {
    mp3Serial.write(CMD_getPlayStatus, 5);
    delay(50);
    return getStatus();
}

String MY1690_16S::getStatus() {
    String statusMp3 = "";
    while (mp3Serial.available()) {
        statusMp3 += (char)mp3Serial.read();
    }
    return statusMp3;
}

void MY1690_16S::stopPlay() {
    setPlayMode(4);
    mp3Serial.write(CMD_MusicStop, 5);
    delay(50);
}

void MY1690_16S::setVolume(unsigned char vol) {
    CMD_VolumeSet[3] = vol;
    checkCode(CMD_VolumeSet);
    mp3Serial.write(CMD_VolumeSet, 6);
    delay(50);
}

void MY1690_16S::volumePlus() {
    mp3Serial.write(CMD_VolumePlus, 5);
    delay(50);
}

void MY1690_16S::volumeDown() {
    mp3Serial.write(CMD_VolumeDown, 5);
    delay(50);
}

void MY1690_16S::setPlayMode(unsigned char mode) {
    CMD_PlayMode[3] = mode;
    checkCode(CMD_PlayMode);
    mp3Serial.write(CMD_PlayMode, 6);
    delay(50);
}

void MY1690_16S::checkCode(unsigned char* vs) {
    int val = vs[1];
    int i;
    for (i = 2; i < vs[1]; i++) {
        val = val ^ vs[i];
    }
    vs[i] = val;
}

void MY1690_16S::ampMode(int p, bool m) {
    pinMode(p, OUTPUT);
    if (m) {
        digitalWrite(p, HIGH);
    } else {
        digitalWrite(p, LOW);
    }
}

void MY1690_16S::init() {
  Serial.println("ooo");
    ampMode(HT6871_PIN, HIGH);
    stopPlay();
    volume = 15;
}
