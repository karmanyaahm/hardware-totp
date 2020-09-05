#define MIN_TIME 1599072502
#include <Wire.h>
#define myWire TwoWire
#define I2C Wire
#include <RtcDS3231.h>

RtcDS3231<myWire> Rtc(I2C);

class timer
{
private:
  time_t now = (time_t)MIN_TIME;
  bool valid = true;

public:
  bool timeSet = false;
  void init();
  unsigned long getTime();
  void setTime(unsigned long t);
  bool isValid();

  timer();
};

timer::timer()
{
  Rtc.Begin();
}

void timer::init()
{
  Serial.println("1");
  if (!Rtc.GetIsRunning())
  {
    //if rtc not running
    Rtc.SetIsRunning(true);
  }

  now = (time_t)Rtc.GetTime();
  Serial.println(now);
  delay(300);
  if ((!isValid()) & (now < MIN_TIME))
  {
    now = (time_t)MIN_TIME;
    Rtc.SetTime(&now);
    valid = false;
  }
  delay(300);
  now = (time_t)Rtc.GetTime();
  Serial.println(now);
  delay(300);
  Serial.println("1");

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  Serial.println("1");
}
unsigned long timer::getTime()
{
  if (!isValid())
  {
    Serial.println(F("getfailed"));
    return 0;
  }
  now = Rtc.GetTime();
  return (unsigned long)now;
}
void timer::setTime(unsigned long t)
{
  now = (time_t)t;
  Rtc.SetTime(&now );
  valid = true;
}
bool timer::isValid()
{
  return valid & Rtc.IsDateTimeValid();
}
timer myTimer;

