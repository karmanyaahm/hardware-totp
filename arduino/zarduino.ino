
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
    Serial.println(sizeof(keys));
    Serial.println(F("setup"));

    Serial.println(sizeof(myData));
    Serial.println(sizeof(myTimer));
}

void loop()
{
    if (pressed)
    {
        myData.toScreen();
        pressed = false;
    }
    unsigned long now = myTimer.getTime();
    char newCode[6];
    myData.getCode(newCode, now);
    //Serial.println(newCode);

    uint8_t interval = now % 30;

    if (strcmp(code, newCode) != 0)
    {
        strcpy(code, newCode);
        myScreen.homeScreen(myData.name, code);
        Serial.println(code);
    }

    myScreen.lineUpdate(interval);
    delay((interval * 1000) % 500);
    delay(1000);
    if (Serial.available())
    {
        processSyncMessage();
    }
}