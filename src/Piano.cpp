#include "Piano.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "print.h"

AudioSynthKarplusStrong   piano1;
AudioMixer4               mixer1;
AudioOutputI2S            i2s1;
AudioConnection           patchCord1(piano1, 0, mixer1, 0);
AudioConnection           patchCord2(mixer1, 0, i2s1, 0);
AudioConnection           patchCord3(mixer1, 0, i2s1, 1);
AudioControlSGTL5000      sgtl5000_1;

struct Note {
    const char* name;
    float frequency;
};

const Note notes[] = {
    {"C1", 32.703}, {"D1", 36.708}, {"E1", 41.203}, {"F1", 43.654}, {"G1", 48.999},
    {"A1", 55.000}, {"B1", 61.735},
    {"C2", 65.406}, {"D2", 73.416}, {"E2", 82.407}, {"F2", 87.307}, {"G2", 97.999},
    {"A2", 110.000}, {"B2", 123.471},
    {"C3", 130.813}, {"D3", 146.832}, {"E3", 164.814}, {"F3", 174.614}, {"G3", 195.998},
    {"A3", 220.000}, {"B3", 246.942},
    {"C4", 261.626}, {"D4", 293.665}, {"E4", 329.628}, {"F4", 349.228}, {"G4", 391.995},
    {"A4", 440.000}, {"B4", 493.883},
    {"C5", 523.251}, {"D5", 587.330}, {"E5", 659.255}, {"F5", 698.456}, {"G5", 783.991},
    {"A5", 880.000}, {"B5", 987.767},
    {"C6", 1046.502}, {"D6", 1174.659}, {"E6", 1318.510}, {"F6", 1396.913}, {"G6", 1567.982},
    {"A6", 1760.000}, {"B6", 1975.533},
    {"C7", 2093.005}, {"D7", 2349.318}, {"E7", 2637.020}, {"F7", 2793.826}, {"G7", 3135.963},
    {"A7", 3520.000}, {"B7", 3951.066}
};

void Piano_begin() {
    AudioMemory(15);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.5);
}

float getNoteFrequency(char key, int num) {
    int noteIndex = -1;
    switch (key) {
        case 'c': noteIndex = 0; break;
        case 'd': noteIndex = 1; break;
        case 'e': noteIndex = 2; break;
        case 'f': noteIndex = 3; break;
        case 'g': noteIndex = 4; break;
        case 'a': noteIndex = 5; break;
        case 'b': noteIndex = 6; break;
        default: return -1;
    }
    int arrayIndex = (num - 1) * 7 + noteIndex;
    if (arrayIndex < 0 || arrayIndex >= (sizeof(notes)/sizeof(notes[0]))) {
        return -1;
    }
    return notes[arrayIndex].frequency;
}

int playNote(char keyInput) {
    switch (keyInput) {
        case 'c': printKeyInput('z'); piano1.noteOn(getNoteFrequency('c', 4), 1.0); return 1;
        case 'd': printKeyInput('x'); piano1.noteOn(getNoteFrequency('d', 4), 1.0); return 1;
        case 'e': printKeyInput('c'); piano1.noteOn(getNoteFrequency('e', 4), 1.0); return 1;
        case 'f': printKeyInput('v'); piano1.noteOn(getNoteFrequency('f', 4), 1.0); return 1;
        case 'g': printKeyInput('b'); piano1.noteOn(getNoteFrequency('g', 4), 1.0); return 1;
        case 'a': printKeyInput('n'); piano1.noteOn(getNoteFrequency('a', 4), 1.0); return 1;
        case 'b': printKeyInput('m'); piano1.noteOn(getNoteFrequency('b', 4), 1.0); return 1;
        default: return 0;
    }
}