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
long long CurrentmusicTime = 0;
long long startmusicTime = 0;
bool rectorder = false;


void loop() {
  if (Serial.available()) {
    keyInput = Serial.read();
    PlayNoteFromKeyInput(keyInput);

    //recorder mode toggle
    if(keyInput == 'r')
    {
      if(rectorder == true)
      {
        // reset recorder and set up for new recording
        
        rectorder = false;
      }
      else
      {
        // start recording new notes
        startmusicTime = millis();
        rectorder = true; 
      }
      RecorderStatus(rectorder);
    }

    // recorder recording notes
    if(rectorder == true)
    {
      notes.push_back(Note(keyInput, 4, millis() - startmusicTime));
    }


  }
}