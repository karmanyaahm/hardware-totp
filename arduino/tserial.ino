
const char TIME_HEADER = 'T';
const char GET_TIME_HEADER = 'G';
const char ADD_HEADER = 'A';
const char NAME_HEADER = 'N';
const char CODE_HEADER = 'C';
const char LIST_HEADER = 'L';
const char REMOVE_HEADER = 'R';
const char SAVE_HEADER = 'S';

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
    Serial.println("setup");

    // uint8_t newCode[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
    // char newName[] = "2344444";
    // myData.easyAdd(newName, newCode);
    // myData.easyAdd(newCode, newName);

    // eepromStore();
    eepromRead();

    myData.init();
}
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
    delay((interval * 1000) % 500);
    delay(300);
    if (Serial.available())
    {
        processSyncMessage();
    }
}
void processSyncMessage()
{
    unsigned long pctime;
    const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
    char b = char(Serial.read());
    if (b == TIME_HEADER)
    {
        pctime = Serial.parseInt();
        if (pctime >= MIN_TIME)
        { // check the integer is a valid time (greater than Jan 1 2013)
            myTimer.setTime(pctime);
            delay(300);
            Serial.println(myTimer.getTime());
        }
    }
    else if (b == GET_TIME_HEADER)
    {
        Serial.println(myTimer.getTime());
    }
    else if (b == LIST_HEADER)
    {
        for (int i = 0; i <= myData.max; i++)
        {
            if (i > 0)
                Serial.print(";");
            Serial.print(keys[i].name);
        }
        Serial.println();
    }
    else
    {
        Serial.println("N");
    }
    serialFlush();
    Serial.println("O");
}
void serialFlush()
{
    while (Serial.available() > 0)
    {
        char t = Serial.read();
    }
}