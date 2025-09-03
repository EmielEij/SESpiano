#pragma once
#include <Arduino.h>

// Initialize piano system (Audio setup, SGTL5000, etc.)
void Piano_begin();

// Handle key input (e.g. 'z', 'x', 'c' ...), plays note and returns 1 if valid
int playNote(char keyInput);
