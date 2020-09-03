#define MIN_TIME 1599072502
#include <Wire.h>
#define myWire TwoWire
#define I2C Wire
#include <RtcDS3231.h>

RtcDS3231<myWire> Rtc(I2C);

class timer
{
  private:
    time_t now = (time_t) MIN_TIME;

    bool valid = true;

  public:
    bool timeSet = false;

    unsigned long getTime();
    void setTime(unsigned long t);
    bool isValid();
    timer();
};

timer::timer()
{
  Serial.println("t");
  Rtc.Begin();
  Serial.println("t");

  if (!Rtc.GetIsRunning())
  {
    //if rtc not running
    Rtc.SetIsRunning(true);
  } Serial.println("t");


  now = (time_t)Rtc.GetTime(); Serial.println("tim");

  if ((!isValid()) & (now < MIN_TIME))
  {
    now = (time_t)MIN_TIME;
    Rtc.SetTime(&now);
    valid = false;
  }
  Serial.println("tim");

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}
unsigned long timer::getTime()
{
  if (isValid())
  {
    return 0;
    Serial.println(F("getfailed"));
  }
  now = Rtc.GetTime();
  return (unsigned long)now;
}
void timer::setTime(unsigned long t)
{
  now = (time_t)MIN_TIME;
  Rtc.SetTime(&now);
  valid = true;
}
bool timer::isValid()
{
  valid &= Rtc.IsDateTimeValid();
  return valid;
}
