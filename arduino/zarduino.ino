
#include <LowPower.h>

const byte BUTTON_PIN = 2;
volatile bool pressed = false;
volatile bool justSlept = false;
volatile unsigned long bounce;
const uint8_t bounce_time = 150;

volatile unsigned long lastPress;
void press()
{
    if ((millis() - bounce) > bounce_time)
    {

        Serial.println(F("press"));

        pressed = (true && (!justSlept));
        bounce = millis();
    }
}

void off()
{
    myScreen.off();
    justSlept = true;
    delay(50);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void on()
{
    myScreen.homeScreen(myData.name, code);
}

void setup()
{
    ADCSRA = 0;

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), press, FALLING);
    Serial.begin(SERIAL_SPEED);
    while (!Serial)
    {
        ;
    }

    myTimer.init();
    if (!myTimer.isValid())
    {
        myTimer.setTime(100);
    }
    myScreen.init();

    // uint8_t newCode[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
    // char newName[] = "2344444";
    // myData.easyAdd(newName, newCode,10);
    // myData.easyAdd(newCode, newName,10);

    // eepromStore();
    eepromRead();

    myData.init();
    lastPress = myTimer.getTime();
    Serial.println(F("setup"));
}

void loop()
{
    unsigned long now = myTimer.getTime();

    if (pressed)
    {
        myData.toScreen();
        pressed = false;
        lastPress = now;
    }
    else if (justSlept)
    {
        on();
        justSlept = false;
        lastPress = now;
    }
    char newCode[6];
    myData.getCode(newCode, now);

    uint8_t interval = now % 30;

    if (strcmp(code, newCode) != 0)
    {
        strcpy(code, newCode);
        myScreen.homeScreen(myData.name, code);
    }

    myScreen.lineUpdate(interval);
    delay(1000);
    if (Serial.available())
    {
        processSyncMessage();
    }
    if (now - lastPress > SLEEP_THRESHOLD)
    {
        off();
    }
}
