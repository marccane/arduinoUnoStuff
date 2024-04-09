#define BUZZER_PIN 11 // The Arduino pin connected to the buzzer
#define LED_PIN 2

FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

const PROGMEM float melodyFreq[] = {
  330,1,494,1,660,1,590,660,494,494,523,1,440,1,494,1,392,392,440,370,1,392,1,330,330,370,1,294,1,330,247,247,261,1,311,1,330,1,247,247,262,1,370,1,330,1,494,1,660,1,590,660,494,494,523,1,440,1,494,1,392,392,440,370,1,392,1,330,330,370,1,294,1,330,247,247,261,1,311,1,330,1,247,247,262,1,370,1,330
};

const PROGMEM unsigned melodyLengths[] = {
  150,40,159,40,150,40,150,150,100,100,150,40,150,40,150,40,100,100,150,150,40,150,40,100,100,150,40,150,40,150,100,100,150,40,150,40,150,40,100,100,150,40,150,40,150,40,159,40,150,40,150,150,100,100,150,40,150,40,150,40,100,100,150,150,40,150,40,100,100,150,40,150,40,150,100,100,150,40,150,40,150,40,100,100,150,40,150,40,150,40
};

void setup() {
  Serial.begin(115200);
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE); // cf https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
  stdout = &f_out;

  printf("sizeof unsigned = %d", sizeof(unsigned));
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // iterate over the notes of the melody:
  int size = sizeof(melodyFreq) / sizeof(float);
  int size2 = sizeof(melodyLengths) / sizeof(unsigned);

  if(size+1 != size2){
    printf("Problemo de lengths! %d %d\n", size, size2);
    tone(BUZZER_PIN, 500, 1000); //Error beep
  }
  else
  {
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      // int noteDuration = 1000 / noteDurations[thisNote];
      unsigned noteFreq = pgm_read_float_near(melodyFreq + thisNote);
      //(unsigned)melodyFreq[thisNote]

      unsigned long noteLen = pgm_read_word_near(melodyLengths + thisNote);
      // unsigned long noteLen = 100;
      tone(BUZZER_PIN, noteFreq, noteLen);

      digitalWrite(LED_PIN, noteFreq > 1000 ? 1 : 0);
      digitalWrite(LED_BUILTIN, noteFreq > 900 ? 1 : 0);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      // int pauseBetweenNotes = 122 * 1.30;
      // delay(pauseBetweenNotes);
      delay(noteLen);
      // stop the tone playing:
      // noTone(BUZZER_PIN);
    }
  }
}

int i = 0;
bool blink = false;
void loop() {
  blink = !blink;
  digitalWrite(LED_PIN, blink);
  printf("Loop %d\n", i++);
  delay(200);
}
