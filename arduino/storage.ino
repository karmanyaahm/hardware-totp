#define BUTTON_PIN 2
volatile bool pressed = false;

void press()
{
    Serial.println("press");
    pressed = true;
}

void setup()
{

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
        myTimer.setTime(1599179228);
    }
    myScreen.init();
    myData.init();
    Serial.println("setup");

    uint8_t newCode[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
    char newName[] = "2344444";
    myData.easyAdd(newCode, newName);
}

char code[7];

void loop()
{
    if (pressed)
    {
        myData.toScreen();
        pressed = false;
    }
    long now = myTimer.getTime();
    char *newCode = myData.getCode(now);

    int interval = now % 30;
    if (strcmp(code, newCode) != 0)
    {
        strcpy(code, newCode);
        myScreen.homeScreen(myData.name, code);
    }

    myScreen.lineUpdate(interval);
    //Serial.println((interval % 3) * 1000);
    //delay(((interval % 3)) * 1000);
    delay(500);
}
