#define CODE_LENGTH 4
#define NUM_BUTTONS 5

enum Buttons
{
  Red,
  Yellow,
  Green,
  Blue,
  Reset,
  Nothing
};


/***********************************************************************
 * Port Definitions
 ***********************************************************************/
const int SSEG_COM1 = 32;
const int SSEG_COM2 = 26;
const int SSEG_COM3 = 24;
const int SSEG_COM4 = 23;
const int SSEG_A = 30;
const int SSEG_B = 22;
const int SSEG_C = 27;
const int SSEG_D = 31;
const int SSEG_E = 33;
const int SSEG_F = 28;
const int SSEG_G = 25;
const int SSEG_DP = 29;
const int BUTTON_A = 53;
const int BUTTON_B = 51;
const int BUTTON_C = 49;
const int BUTTON_D = 47;
const int BUTTON_E = 45;
const int LED_A = 52;
const int LED_B = 50;
const int LED_C = 48;
const int LED_D = 46;

/*************************************************
 * Public Constants
 *************************************************/

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


struct InputCode
{
  char code[CODE_LENGTH];
  // Number of remaining buttons to input
  char remaining_input = CODE_LENGTH;
  bool button_pressed[CODE_LENGTH];
};


InputCode bomb_code;
InputCode user_attempt;


void setup() {
  // put your setup code here, to run once:
  setup_bomb();
  setup_buttons_leds();
  setup_display_pins();
  setup_display_refresh_timer();
  start_countdown_timer(10);
  
}

void loop() {

// put your main code here, to run repeatedly:


  // checks if there is button input and turns lights on
  int button = check_buttons();
  update_countdown_timer();

  if (button != Nothing)
  {
    handle_button(button);
  }

  if (countdown_timer_expired())
  {
    turn_off_lights();
    bomb_explosion();

  }
  

  if ((user_attempt.remaining_input) == 0){
    // check if the code is correct
    if (is_code_correct())
    {
      // decoded
      // celebration!!
      defused_bomb_sound();
      
      while(true)
      {

      }

      //something
    }
    else // code is wrong
    {
      wrong_code_sound();
      delay(100);
      turn_off_lights();
      reset_digits();
      delay(200);
      // make annoying noise
    }

  }
  

}


// Helper functions

// setup/initialisation

void setup_bomb()
{
  
  bomb_code.code[0] = Red;
  bomb_code.code[1] = Blue;
  bomb_code.code[2] = Yellow;
  bomb_code.code[3] = Green;
}

bool is_code_correct()
{
  for (unsigned char i = 0; i < CODE_LENGTH; i++)
  {
    if (bomb_code.code[i] != user_attempt.code[i])
      return false;
  }
  return true;
}

void setup_buttons_leds()
{
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
  pinMode(BUTTON_D, INPUT);
  pinMode(BUTTON_E, INPUT);
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
}

// Return the button that has been pressed
char check_buttons()
{
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (read_button(i))
      return i;
  }
  return Nothing;
}

bool read_button(char button)
{
  switch (button)
  {
    case 0: return digitalRead(BUTTON_A) == HIGH;
    case 1: return digitalRead(BUTTON_B) == HIGH;
    case 2: return digitalRead(BUTTON_C) == HIGH;
    case 3: return digitalRead(BUTTON_D) == HIGH;
    case 4: return digitalRead(BUTTON_E) == HIGH;
    default: return false;
  }
}

// Resets button if reset has been pressed
// otherwise, enters digit
void handle_button(char button)
{
  if (button == Reset)
    reset_digits();
    
  else if (!user_attempt.button_pressed[button])  {
    enter_digit(button);
    turn_on_light(button);
  }
  
}


void enter_digit(char which_button)
{
  char digit_location = CODE_LENGTH - (user_attempt.remaining_input);
  user_attempt.code[digit_location] = which_button;
  user_attempt.button_pressed[which_button] = true;
  (user_attempt.remaining_input)--;  
}

void reset_digits()
{
  user_attempt.remaining_input = CODE_LENGTH;

  for (int i = 0; i < CODE_LENGTH; i++)
  {
    user_attempt.button_pressed[i] = false;
  }
  
  turn_off_lights();
}

void turn_off_lights()
{
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
  digitalWrite(LED_D, LOW);
}

void turn_on_light(char light)
{  
  switch(light)
  {
    case Red:
      digitalWrite(LED_A, HIGH);
      //delay(100);
      //digitalWrite(LED_A, LOW);
      break;
    case Yellow: 
      digitalWrite(LED_B, HIGH);
      //delay(100);
      //digitalWrite(LED_B, LOW);
      break;
    case Green: 
      digitalWrite(LED_C, HIGH);
      //delay(100);
      //digitalWrite(LED_C, LOW);
      break;
    case Blue: 
      digitalWrite(LED_D, HIGH);
      //delay(100);
      //digitalWrite(LED_D, LOW);
      break;      
  }
  
}


/***********************************************************************
 * Display Stuff
 ***********************************************************************/
static volatile unsigned char display_segment_state[4]; /* zero */

ISR(TIMER5_COMPA_vect)
{
  for (int i = 0; i < 5; i++)
  {
    //if (val == 0)
      //continue;

    // clear
    digitalWrite(SSEG_A, HIGH);
    digitalWrite(SSEG_B, HIGH);
    digitalWrite(SSEG_C, HIGH);
    digitalWrite(SSEG_D, HIGH);
    digitalWrite(SSEG_E, HIGH);
    digitalWrite(SSEG_F, HIGH);
    digitalWrite(SSEG_G, HIGH);
    digitalWrite(SSEG_DP, HIGH);    

    // select
    digitalWrite(SSEG_COM1, (i == 0));
    digitalWrite(SSEG_COM2, (i == 1));
    digitalWrite(SSEG_COM3, (i == 2));
    digitalWrite(SSEG_COM4, (i == 3));
    if (i == 4)
      break;

    unsigned char val = display_segment_state[i];

    // display
    digitalWrite(SSEG_A, (val & 1) == 0);
    digitalWrite(SSEG_B, (val & 2) == 0);
    digitalWrite(SSEG_C, (val & 4) == 0);
    digitalWrite(SSEG_D, (val & 8) == 0);
    digitalWrite(SSEG_E, (val & 16) == 0);
    digitalWrite(SSEG_F, (val & 32) == 0);
    digitalWrite(SSEG_G, (val & 64) == 0);
    digitalWrite(SSEG_DP, (val & 128) == 0);
  }
}

void setup_display_pins()
{
  pinMode(SSEG_COM1, OUTPUT);
  pinMode(SSEG_COM2, OUTPUT);
  pinMode(SSEG_COM3, OUTPUT);
  pinMode(SSEG_COM4, OUTPUT);
  pinMode(SSEG_A, OUTPUT);
  pinMode(SSEG_B, OUTPUT);
  pinMode(SSEG_C, OUTPUT);
  pinMode(SSEG_D, OUTPUT);
  pinMode(SSEG_E, OUTPUT);
  pinMode(SSEG_F, OUTPUT);
  pinMode(SSEG_G, OUTPUT);
  pinMode(SSEG_DP, OUTPUT);

  // 7-seg is active low, so start high (off)
  digitalWrite(SSEG_COM1, LOW);
  digitalWrite(SSEG_COM2, LOW);
  digitalWrite(SSEG_COM3, LOW);
  digitalWrite(SSEG_COM4, LOW);
  digitalWrite(SSEG_A, HIGH);
  digitalWrite(SSEG_B, HIGH);
  digitalWrite(SSEG_C, HIGH);
  digitalWrite(SSEG_D, HIGH);
  digitalWrite(SSEG_E, HIGH);
  digitalWrite(SSEG_F, HIGH);
  digitalWrite(SSEG_G, HIGH);
  digitalWrite(SSEG_DP, HIGH);
}

void setup_display_refresh_timer()
{
  TCNT5 = 0;
  TCCR5B = (1 << CS52);
  //TIFR5 |= (1 << TOV5);
  //TIMSK5 |= (1 << TOIE5);

  TIFR5 |= (1 << OCF5A);
  TIMSK5 |= (1 << OCIE5A);
  OCR5A = 35000;
}

void set_display_digit(int seg, int number)
{
  static const unsigned char number_lookup[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00100111,
    0b01111111,
    0b01101111  
  };

  display_segment_state[seg] = number_lookup[number];
}

void set_time(int total_time)
{
  int minutes = total_time / 60;
  int seconds = total_time % 60;
  //set_display_digit(0, minutes / 10);
  //set_display_digit(1, minutes % 10);
  //set_display_digit(2, seconds / 10);
  //set_display_digit(3, seconds % 10);
  set_display_digit(0, (minutes / 10) % 10);
  set_display_digit(1, (minutes) % 10);
  set_display_digit(2, (seconds / 10) % 10);
  set_display_digit(3, seconds % 10);
}

/***********************************************************************
 * Countdown timer
 ***********************************************************************/
static unsigned long countdown_time_remaining = 0;
static unsigned long countdown_last_millis = 0;

void start_countdown_timer(int seconds)
{
  countdown_time_remaining = (unsigned long)(seconds + 1) * 1000;
  countdown_last_millis = millis();
}

void update_countdown_timer()
{
  if (countdown_time_remaining == 0)
    return;
  
  unsigned long val = millis();
  unsigned int diff = val - countdown_last_millis;
  if (diff == 0)
    return;

  if (diff <= countdown_time_remaining)
    countdown_time_remaining -= diff;
  else
    countdown_time_remaining = 0;

  countdown_last_millis = millis();
  set_time(countdown_time_remaining / 1000);
}

bool countdown_timer_expired()
{
  return countdown_time_remaining == 0;
}

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void defused_bomb_sound()
{
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(41, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(41);
  }
}

void bomb_explosion()
{
  TIMSK5 = 0;
  
  while (true){
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / 4;
    tone(41, NOTE_B3, noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(41);
  }
}

void wrong_code_sound()
{
  int noteDuration = 1000 / 4;
  tone(41, NOTE_B3, noteDuration);

  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = noteDuration * 1.30;
  delay(pauseBetweenNotes);
  // stop the tone playing:
  noTone(41);
}


