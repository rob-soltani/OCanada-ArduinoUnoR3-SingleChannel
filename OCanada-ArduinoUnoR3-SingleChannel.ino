/*

    Ryerson University
    G. Raymond Chang School of Continuing Education

    CCPS 310 (1J0) – Computer Organization II – Winter 2020
    Instructor: Alexander Ufkes (https://www.linkedin.com/in/alex-ufkes-37154844/)
    Lab 6 - Lab 6 – AVR III: Program a piece of music for the Arduino Uno Board to play on the passive buzzer

    Version 1: O'Canada, single-line melody; buzzer on pin 9 of the Arduino Uno Board.

    Melody is encoded by me, Rob (Sohrab) Soltani, in April 2020 (https://rob.soltani.io).
    Melody is based on the score downloaded from: http://cantorion.org/music/3873/O-Canada-French-lyrics.

*/

// pitches.h provided by Alexander Ufkes
# include "pitches.h"

// Processor frequency divided by 8 (because using an 8-bit counter?)
#define BUZZER_CLOCK 16000000/8

// timer_init initializes time/counter control registers TCCR1A and TCCR1B
// Function sets appropriate bits in the registers
// Function was provided by Alexander Ufkes
void timer_init(void)
{

   // Using ARC Assembly language, set the bit for pin 9
   // on the DDRB register which is port B's data direction register (0x04).
   // This sets the pin as output.
   asm(" sbi 0x04, 1 ");

   // Set appropriate bits in the time/counter control register TCCR1A
   TCCR1A = (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11);

   // Set appropriate bits in the time/counter control register TCCR1B
   TCCR1B = (1 << WGM13)  | (1 << WGM12)  | (1 << CS11);
}

// tone accepts a freq parameter based on which it sets the
// ICR1 and OCR1A registers
// Function was provided by Alexander Ufkes
void tone(uint16_t freq)
{
   // Again, set the bit for pin 9 on port B's data direction register (0x04)
   // because for some notes, and silence at the end, the mute function clears
   // the bit, so we need it set again to play tones.
   asm(" sbi 0x04, 1 ");

   // Set the value of the Input Capture Register based on the input frequency.
   ICR1 = BUZZER_CLOCK/(freq*2);

   // Set the value of the OCR1A register which is connected to pin 9.
   OCR1A = ICR1*3/4;

   // Based on the bit configurations set for TCCR1A and TCCR1B registers,
   // the ICR1 and OCR1A control the duty cycle and frequency of
   // waveform generation.

   // After enccoding O Canada (the melody array below), I
   // realized it sounded better to my ears when it was one octave higher;
   // that is why freq is multiplied by two here.
}

// The mute function achive silence and silent notes when needed
// by clearing the pin 9 output and clearing the data direction bit.
// Function was provided by Alexander Ufkes
void mute(void)
{
  // Clear the LSB bit on port B's data direction register (0x04),
  // to stop output tone.
  asm(" cbi 0x04, 1 ");

  // Also, clear the LSB bit on port B's data register (0x05),
  // to stop output tone.
  asm(" cbi 0x05, 1 ");
}

// The ususal Arduino setup function
// Function was provided by Alexander Ufkes
void setup()
{
  timer_init();
}

// The general template of the following three arrays and the loop Function
// was provided by Alexander Ufkes

/*
   An array to hold the notes for O Canada
   These are encoded by me.
   Zeroes mean silence or silent notes (when the mute is called).
*/
/*
   I divided the melody into what I call sentences (is that a valid music term?),
   which to me are musical statements that have musical meaning with a start,
   and an end. This helped me be able to listen and debug the music better.
   Each of these musical sentences are represented as a line of notes,
   note durations, and mute durations respectively in the arrays melody,
   noteDurations, and muteDurations below.
*/
uint16_t melody[] =
{
    NOTE_G3,   NOTE_AS3,   NOTE_AS3,   NOTE_DS3,    NOTE_F3,    NOTE_G3,   NOTE_GS3,   NOTE_AS3,    NOTE_C4,    NOTE_F3,
    NOTE_G3,    NOTE_A3,    NOTE_A3,   NOTE_AS3,    NOTE_C4,    NOTE_D4,    NOTE_D4,    NOTE_C4,    NOTE_C4,   NOTE_AS3,
    NOTE_F3,    NOTE_G3,   NOTE_GS3,    NOTE_G3,    NOTE_F3,    NOTE_G3,   NOTE_GS3,   NOTE_AS3,   NOTE_GS3,    NOTE_G3,   NOTE_GS3,   NOTE_AS3,    NOTE_C4,   NOTE_AS3,   NOTE_GS3,   NOTE_G3,   NOTE_F3,
    NOTE_F3,    NOTE_G3,   NOTE_GS3,    NOTE_G3,    NOTE_F3,    NOTE_G3,   NOTE_GS3,   NOTE_AS3,   NOTE_GS3,    NOTE_G3,    NOTE_G3,    NOTE_F3,   NOTE_AS3,   NOTE_AS3,    NOTE_A3,   NOTE_G3,   NOTE_A3,   NOTE_AS3,         0,
    NOTE_G3,   NOTE_AS3,   NOTE_AS3,   NOTE_DS3,          0,   NOTE_GS3,    NOTE_C4,    NOTE_C4,    NOTE_F3,          0,   NOTE_AS3,    NOTE_B3,    NOTE_B3,    NOTE_C4,   NOTE_GS3,   NOTE_G3,   NOTE_F3,   NOTE_DS3,   NOTE_F3,   NOTE_G3,   0,
   NOTE_AS3,   NOTE_DS4,   NOTE_DS4,    NOTE_C4,   NOTE_GS3,    NOTE_G3,    NOTE_F3,   NOTE_AS3,    NOTE_D3,   NOTE_DS3,          0
 };

 // An array to hold the duration of each note for O Canada
 // These are also encoded by me.
uint8_t noteDurations[] =
{
   8, 5, 2, 12, 4, 4, 4, 4, 4, 16,                                       // 10 - 10
   8, 5, 2, 12, 4, 3, 3, 3, 3, 12,                                       // 10 - 20
   3, 1, 6,  2, 4, 3, 1, 6, 2,  4, 3, 1, 3, 3, 3, 3, 12,                 // 17 - 37
   3, 1, 6,  2, 4, 3, 1, 6, 2,  3, 4, 4, 3, 2, 2, 2,  2, 12, 4,          // 19 - 56
   8, 5, 2,  6, 6, 8, 5, 2, 8,  4, 8, 5, 2, 4, 4, 4,  4,  8, 8, 12, 4,   // 21 - 77
   8, 5, 2,  4, 4, 4, 4, 8, 8, 12, 4                                     // 11 - 88
};

// An array to hold the duration of each silent note in the melody array
// These are  encoded by me as well.
double muteDurations[] =
{
   0,   0.20,   0,   0,   0,     0,      0,     0,     0,     0,
   0,   0.20,   0,   0,   0,   0.3,    0.3,   0.3,   0.3,     0,
   0,      0,   0,   0,   0,     0,      0,     0,     0,     0,    0,       0,   0.3,   0.3,   0.3,   0.3,   0.1,
   0,      0,   0,   0,   0,     0,      0,     0,     0,   0.3,    0,       0,   0.3,     0,     0,     0,     0,   0,   0,
   0,   0.20,   0,   0,   0,     0,   0.20,     0,     0,     0,    0,    0.20,     0,     0,     0,     0,     0,   0,   0,   0,   0,
   0,   0.20,   0,   0,   0,     0,      0,     0,     0,     0,    0
};

// The usual Arduino loop.
// This was provided by Alexander Ufkes,
// But I modified it a bit to make the played music
// more pleasant to my ears :)
void loop()
{
  for (int noteID = 0; noteID < 88 ; noteID++)
  {
    int freq = melody[noteID];

    if (freq == 0) mute();
    else tone(freq);

    uint16_t noteDuration = noteDurations[noteID]*80;
    delay(noteDuration);

    mute();

    if (muteDurations[noteID] == 0)
    {
      muteDurations[noteID] = 1/100;
    }

    uint16_t muteDuration = noteDuration * muteDurations[noteID];
    delay(muteDuration);

  }

  delay(1000);

}
