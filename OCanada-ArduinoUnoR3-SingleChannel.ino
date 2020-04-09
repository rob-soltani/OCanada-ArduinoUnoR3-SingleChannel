
/*

    CCPS 310 - Winter 2020 - Lab 6 - Version 1
    Sohrab Soltani, Student #500801172

    Version One: O'Canada, single line melody; buzzer on pin 9

    Melody is encoded by Sohrab Soltani
    Based on the score downloaded from: http://cantorion.org/music/3873/O-Canada-French-lyrics

*/

# include "pitches.h"
#define BUZZER_CLOCK 16000000/8


void timer_init(void) {
   asm(" sbi 0x04, 1 "); // Use PWM pin 9
   TCCR1A = (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11);
   TCCR1B = (1 << WGM13)  | (1 << WGM12)  | (1 << CS11);
}

void tone(uint16_t freq)
{
   asm(" sbi 0x04, 1 ");
   ICR1 = BUZZER_CLOCK/(freq*2);
   OCR1A = ICR1*3/4;
}
void mute(void)
{
  asm(" cbi 0x04, 1 ");
  asm(" cbi 0x05, 1 ");
}


void setup() {
  timer_init();
}

void loop()
{
  uint16_t melody[] = {
    NOTE_G3, NOTE_AS3, NOTE_AS3, NOTE_DS3,   NOTE_F3, NOTE_G3, NOTE_GS3, NOTE_AS3,  NOTE_C4, NOTE_F3,
    NOTE_G3,  NOTE_A3,  NOTE_A3, NOTE_AS3,   NOTE_C4, NOTE_D4,  NOTE_D4,  NOTE_C4,  NOTE_C4, NOTE_AS3,
    NOTE_F3,  NOTE_G3, NOTE_GS3,  NOTE_G3,   NOTE_F3, NOTE_G3, NOTE_GS3, NOTE_AS3, NOTE_GS3,  NOTE_G3, NOTE_GS3, NOTE_AS3,  NOTE_C4, NOTE_AS3, NOTE_GS3, NOTE_G3,  NOTE_F3,
    NOTE_F3,  NOTE_G3, NOTE_GS3,  NOTE_G3,   NOTE_F3, NOTE_G3, NOTE_GS3, NOTE_AS3, NOTE_GS3,  NOTE_G3,  NOTE_G3,  NOTE_F3, NOTE_AS3, NOTE_AS3,  NOTE_A3, NOTE_G3, NOTE_A3, NOTE_AS3,       0,
    NOTE_G3, NOTE_AS3, NOTE_AS3,  NOTE_DS3,        0, NOTE_GS3, NOTE_C4,  NOTE_C4,  NOTE_F3,        0, NOTE_AS3,  NOTE_B3,  NOTE_B3,  NOTE_C4, NOTE_GS3, NOTE_G3, NOTE_F3, NOTE_DS3, NOTE_F3, NOTE_G3, 0,
   NOTE_AS3, NOTE_DS4, NOTE_DS4,   NOTE_C4, NOTE_GS3,  NOTE_G3, NOTE_F3, NOTE_AS3,  NOTE_D3, NOTE_DS3, 0};


  uint8_t noteDurations[] = { 8, 5, 2, 12, 4, 4, 4, 4, 4, 16, // 10 - 10
                              8, 5, 2, 12, 4, 3, 3, 3, 3, 12, // 10 - 20
                              3, 1, 6,  2, 4, 3, 1, 6, 2,  4, 3, 1, 3, 3, 3, 3, 12, // 17 - 37
                              3, 1, 6,  2, 4, 3, 1, 6, 2,  3, 4, 4, 3, 2, 2, 2,  2, 12, 4, // 19 - 56
                              8, 5, 2,  6, 6, 8, 5, 2, 8,  4, 8, 5, 2, 4, 4, 4,  4,  8, 8, 12, 4, // 21 - 77
                              8, 5, 2,  4, 4, 4, 4, 8, 8, 12, 4}; // 11 - 88


  double muteDurations[] = { 0, 0.20, 0, 0, 0,   0,   0,   0,   0,   0,
                             0, 0.20, 0, 0, 0, 0.3, 0.3, 0.3, 0.3,   0,
                             0,    0, 0, 0, 0,   0,   0,   0,   0,   0,    0,    0, 0.3, 0.3, 0.3, 0.3, 0.1,
                             0,    0, 0, 0, 0,   0,   0,   0,   0, 0.3,    0,    0, 0.3,   0,   0,   0,   0, 0, 0,
                             0, 0.20, 0, 0, 0, 0,  0.20,   0,   0,   0,    0, 0.20,   0,   0,   0,   0,   0, 0, 0, 0, 0,
                             0, 0.20, 0, 0, 0, 0, 0, 0, 0, 0, 0};


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
