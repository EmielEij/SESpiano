#include <Arduino.h>
#include <string>
#include <algorithm>
#include <vector>
#include "Piano.h"
#include "print.h"
#include "Bounce2.h"

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

const int PotMeterPin = 39;
float potValue = 0.0;

void CheckVolume() {
  float volume = analogRead(PotMeterPin);   
  float lastVolume = 0.0; 
  
  if (volume != lastVolume) {
    setvolume(volume * 0.8 / 1023.0);
    lastVolume = volume; 
  }
}

#define BUTTON_DEBOUNCE_DELAY 25

#define BUTTON_PIN_OCTAVE_UP 41 
Bounce ButtonUp = Bounce(BUTTON_PIN_OCTAVE_UP, BUTTON_DEBOUNCE_DELAY);

#define BUTTON_PIN_OCTAVE_DOWN 40
Bounce ButtonDown = Bounce(BUTTON_PIN_OCTAVE_DOWN, BUTTON_DEBOUNCE_DELAY);

#define BUTTON_PIN_CHANGE_INSTRUMENT 38
Bounce ButtonChangeInstrument = Bounce(BUTTON_PIN_CHANGE_INSTRUMENT, BUTTON_DEBOUNCE_DELAY);

int octave = 1; 

void CheckOctave() {
  ButtonUp.update();
  ButtonDown.update(); 

  if (ButtonUp.fell() && octave < 7) {
    octave++;
    OctaveStatus(octave);
  }

  if (ButtonDown.fell() && octave > 1) {
    octave--; 
    OctaveStatus(octave);
  }
}

char noteFromKeyInput(char keyInput) {
  switch (keyInput) {
  case 'z': return 'c';
  case 'Z': return 'c';
  case 'x': return 'd';
  case 'X': return 'd';
  case 'c': return 'e';
  case 'C': return 'e';
  case 'v': return 'f';
  case 'V': return 'f';
  case 'b': return 'g';
  case 'B': return 'g';
  case 'n': return 'a';
  case 'N': return 'a';
  case 'm': return 'b';
  case 'M': return 'b';
  default: return ' ';
  }
}

void PlayNoteFromKeyInput(char keyInput)
{
  switch (keyInput) {
  case 'z': playNote('c', octave); break;
  case 'x': playNote('d', octave); break;
  case 'c': playNote('e', octave); break;
  case 'v': playNote('f', octave); break;
  case 'b': playNote('g', octave); break;
  case 'n': playNote('a', octave); break;
  case 'm': playNote('b', octave); break;
  default: break;
  }
}

std::vector<Note> notes;

uint32_t currentTime = 0;       // global timeline
uint32_t msPerStep = 90; // resolution try 166 or 200 for timing

// updated parseSongLine with measureStart
void parseSongLine(const std::string &pattern, int octave,
                   std::vector<Note> &notes, uint32_t measureStart) {
    for (size_t i = 0; i < pattern.size(); i++) {
        char c = pattern[i];
        if (c != '-' && c != '|') {
            uint32_t noteTime = measureStart + i * msPerStep;
            notes.emplace_back(c, octave, noteTime);
        }
    }
}



void setup() {
  Serial.begin(115200);
  while (!Serial);

  ButtonUp.attach(BUTTON_PIN_OCTAVE_UP, INPUT_PULLUP); 
  ButtonDown.attach(BUTTON_PIN_OCTAVE_DOWN, INPUT_PULLUP);

  Piano_begin();
    uint32_t measureStart;
    // ===== Measure 1 =====
    measureStart = currentTime;
    parseSongLine("--d---------------d-------", 5, notes, measureStart);
    parseSongLine("dd--a--G-g-f-dfgcc--a--G-g", 4, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 2 =====
    measureStart = currentTime;
    parseSongLine("--------d---------------d-", 5, notes, measureStart);
    parseSongLine("-f-dfg----a--G-g-f-dfg----", 4, notes, measureStart);
    parseSongLine("------bb--------------AA--", 3, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 3 =====
    measureStart = currentTime;
    parseSongLine("--------------d-----------", 5, notes, measureStart);
    parseSongLine("a--G-g-f-dfgdd--a--G-g-f-d", 4, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 4 =====
    measureStart = currentTime;
    parseSongLine("----d---------------d-----", 5, notes, measureStart);
    parseSongLine("fgcc--a--G-g-f-dfg----a--G", 4, notes, measureStart);
    parseSongLine("------------------bb------", 3, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 5 =====
    measureStart = currentTime;
    parseSongLine("----------d---------------", 5, notes, measureStart);
    parseSongLine("-g-f-dfg----a--G-g-f-dfgdd", 4, notes, measureStart);
    parseSongLine("--------AA----------------", 4, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 6 =====
    measureStart = currentTime;
    parseSongLine("d---------------d---------", 5, notes, measureStart);
    parseSongLine("--a--G-g-f-dfgcc--a--G-g-f", 4, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 7 =====
    measureStart = currentTime;
    parseSongLine("------d---------------d---", 5, notes, measureStart);
    parseSongLine("-dfg----a--G-g-f-dfg----a-", 4, notes, measureStart);
    parseSongLine("----bb--------------AA----", 3, notes, measureStart);
    currentTime += 26 * msPerStep;

    // ===== Measure 8 =====
    measureStart = currentTime;
    parseSongLine("-------------d------------", 5, notes, measureStart);
    parseSongLine("-G-g-f-dfgdd--a--G-g-f-dfg", 4, notes, measureStart);
    currentTime += 26 * msPerStep;

//-------------------------------------------------------------
  // msPerStep = 166;
  // // ===== Measure 1 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("C-----d-e---e---e---a---a-", 4, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 2 =====
  //   measureStart = currentTime;
  //   parseSongLine("--C---C-------------------", 5, notes, measureStart);
  //   parseSongLine("------------b-a-----------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 3 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("----G---a---b-------------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 4 =====
  //   measureStart = currentTime;
  //   parseSongLine("--C-----------------------", 5, notes, measureStart);
  //   parseSongLine("--------b-a---------------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 5 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("C-----d-e---e---e---a---a-", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 6 =====
  //   measureStart = currentTime;
  //   parseSongLine("--C---C-------------------", 5, notes, measureStart);
  //   parseSongLine("------------b-a-----------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 7 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("----G---a---b-------------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 8 =====
  //   measureStart = currentTime;
  //   parseSongLine("--C-----------------------", 5, notes, measureStart);
  //   parseSongLine("--------b-a---------------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 9 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("e-----a-G---G---G---G---F-", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 10 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("--G---a---------------G---", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 11 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------e---d-", 5, notes, measureStart);
  //   parseSongLine("--a-b---b---b---b---------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;  

  //   // ===== Measure 12 =====
  //   measureStart = currentTime;
  //   parseSongLine("--C-----------------------", 5, notes, measureStart);
  //   parseSongLine("------------------C-----d-", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 13 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------C---C-", 5, notes, measureStart);
  //   parseSongLine("e---e---e---a---a---------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 14 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("----b-a---------------G---", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 15 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------C-----", 5, notes, measureStart);
  //   parseSongLine("a---b---------------------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;

  //   // ===== Measure 16 =====
  //   measureStart = currentTime;
  //   parseSongLine("--------------------------", 5, notes, measureStart);
  //   parseSongLine("b-a-----------------------", 4, notes, measureStart);
  //   parseSongLine("--------------------------", 3, notes, measureStart);
  //   currentTime += 26 * msPerStep;




  std::sort(notes.begin(), notes.end(),
          [](const Note &a, const Note &b) {
              if (a.timing == b.timing) return a.octave < b.octave;
              return a.timing < b.timing;
          });

    
}


char keyInput;



int currentPlaybackNote = 0;
long long playbackStartTime = 0;
long long playbackCurrentTime = 0;
long long CurrentmusicTime = 0;
long long startmusicTime = 0;
bool rectorder = false;
bool playback = false;


void loop() {
  CheckVolume(); 
  CheckOctave(); 

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
        Serial.println("playback");
        for (auto note : notes) {
          Serial.print(note.name);
          Serial.print(" ");
          Serial.print(note.octave);
          Serial.print(" ");
          Serial.println(note.timing);
        }
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
      notes.push_back(Note(noteFromKeyInput(keyInput), octave, millis() - startmusicTime));
    }
  }

  if(playback == true)
  {
    if(notes[currentPlaybackNote].timing + playbackCurrentTime <= millis())
    {
      playNote(notes[currentPlaybackNote].name, notes[currentPlaybackNote].octave);
      OctaveStatus(notes[currentPlaybackNote].octave);
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