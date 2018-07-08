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

