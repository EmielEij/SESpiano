#include "print.h"
#include <string>

char currentNote = ' ';
bool recordingStatus = false;
bool playbackStatus = false;

void formatAndPrintString()
{
    std::string s;
    switch (currentNote) {
        case 'z': 
            // Serial.print('\r'); 
            // Serial.print(" Z x c v b n m ");
            s += " Z x c v b n m ";
        break;
        case 'x': 
            s += " z X c v b n m ";
        break;
        case 'c': 
            s += " z x C v b n m ";
        break;
        case 'v': 
            s += " z x c V b n m ";
        break;
        case 'b': 
            s += " z x c v B n m ";
        break;
        case 'n': 
            s += " z x c v b N m ";
        break;
        case 'm': 
            s += " z x c v b n M ";
        break;
        case ' ':
            s += " z x c v b n m ";
        break;
        default:
            s += " z x c v b n m ";
        break;
    }

    if(recordingStatus == true)
    {
        s += "|Recording|";
    }
    else 
    {
        s += "|        |";
    }

    if (playbackStatus == true)
    {
        s += "|Playback|";
    }
    else 
    {
        s += "|        |";
    }

    Serial.print('\r'); 
    Serial.print(s.c_str());
}

void printKeyInput(char keyInput) {
    currentNote = keyInput;
    formatAndPrintString();
}

void RecorderStatus(bool recording)
{
    recordingStatus = recording;
    currentNote = ' ';
    formatAndPrintString();
}

void PlaybackStatus(bool playback)
{
    playbackStatus = playback;
    currentNote = ' ';
    formatAndPrintString();
}