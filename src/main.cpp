#include <Arduino.h>
#include <vector>
#include "Piano.h"
#include "print.h"


struct Note {
    char name;       // single character note: C, D, E, F, G, A, B
    int octave;      // octave number, e.g. 4 = middle C octave
    uint32_t timing; // duration in milliseconds

    // Constructor for convenience
    Note(char n, int o, uint32_t t) {
        name = n;
        octave = o;
        timing = t;
    }
};

char noteFromKeyInput(char keyInput) {
  switch (keyInput) {
  case 'z': return 'c';
  case 'x': return 'd';
  case 'c': return 'e';
  case 'v': return 'f';
  case 'b': return 'g';
  case 'n': return 'a';
  case 'm': return 'b';
  default: return ' ';
  }
}

void PlayNoteFromKeyInput(char keyInput)
{
  switch (keyInput) {
  case 'z': playNote('c'); break;
  case 'x': playNote('d'); break;
  case 'c': playNote('e'); break;
  case 'v': playNote('f'); break;
  case 'b': playNote('g'); break;
  case 'n': playNote('a'); break;
  case 'm': playNote('b'); break;
  default: break;
  }
}




void setup() {
  Serial.begin(115200);
  while (!Serial);

  Piano_begin();
}

char keyInput;
std::vector<Note> notes;
int currentPlaybackNote = 0;
long long playbackStartTime = 0;
long long playbackCurrentTime = 0;
long long CurrentmusicTime = 0;
long long startmusicTime = 0;
bool rectorder = false;
bool playback = false;


void loop() {
  if (Serial.available()) {
    keyInput = Serial.read();
    if(playback == false )
    {
      PlayNoteFromKeyInput(keyInput);
    }
    // enable playback of recorded notes
    if(keyInput == 'p' && rectorder == false)
    {
      if(playback == true)
      {
        currentPlaybackNote = 0;
        playback = false;
      }
      else
      {
        playbackCurrentTime = millis();
        playback = true;
      }
      PlaybackStatus(playback);
    }
    //recorder mode toggle
    if(keyInput == 'r' && playback == false)
    {
      if(rectorder == true)
      {
        // // reset recorder and set up for new recording
        //  for (auto note : notes) {
        //   Serial.print(note.name);
        //   Serial.print(" ");
        //   Serial.print(note.octave);
        //   Serial.print(" ");
        //   Serial.println(note.timing);
        // }
        rectorder = false;
      }
      else
      {
        // start recording new notes
        notes.clear();
        startmusicTime = millis();
        rectorder = true; 
      }
      RecorderStatus(rectorder);
    }

    // recorder recording notes
    if(rectorder == true && keyInput != 'p' && keyInput != 'r')
    {
      notes.push_back(Note(noteFromKeyInput(keyInput), 4, millis() - startmusicTime));
    }
  }

  if(playback == true)
  {
    if(notes[currentPlaybackNote].timing + playbackCurrentTime <= millis())
    {
      playNote(notes[currentPlaybackNote].name);
      currentPlaybackNote += 1;
      if(currentPlaybackNote >= notes.size())
      {
        currentPlaybackNote = 0;
        playback = false;
        PlaybackStatus(playback);
      }
    }
  }
}