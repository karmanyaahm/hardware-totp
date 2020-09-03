#define SERIAL_SPEED 9600

#include <TimeLib.h>
#include <Wire.h>
#define myWire TwoWire
#define I2C Wire
#include <RtcDS3231.h>

RtcDS3231<myWire> Rtc(I2C);

class timer
{

public:
    timer();
};

timer::timer(void)
{
}

void setup()
{
    Serial.begin(SERIAL_SPEED);
    Rtc.Begin();

    if (!Rtc.GetIsRunning())
    {
        //if rtc not running
        Rtc.SetIsRunning(true);
    }

    time_t now = (time_t)1599072502;
    Rtc.SetTime(&now);
    delay(1000);

    now = Rtc.GetTime();
    Serial.println(now);
    Serial.println('4');

    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
    Serial.println('5');
}
void loop()
{

    if (Rtc.IsDateTimeValid()) // Check if the RTC is still reliable...
    {

        time_t now = Rtc.GetTime();

        Serial.print(F("Unix time (1970 based):   "));
        Serial.println(now );
    }
    else
    {
        Serial.println("Fail");
    }        delay(1000);

}
