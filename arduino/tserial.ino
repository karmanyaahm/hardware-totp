
const char TIME_HEADER = 'T';
const char GET_TIME_HEADER = 'G';
const char ADD_HEADER = 'A';
const char LIST_HEADER = 'L';
const char REMOVE_HEADER = 'R';
const char SAVE_HEADER = 'S';
const char OUT_SEPERATOR = ';';
const char IN_SEPERATOR = ':';

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
    else if (b == SAVE_HEADER)
    {
        eepromStore();
        Serial.println();
    }
    else if (b == REMOVE_HEADER)
    {
        myData.remove(Serial.parseInt());
    }
    else if (b == ADD_HEADER)
    {
        char newName[10];
        int a = Serial.readBytesUntil(IN_SEPERATOR, newName, 15);
        newName[a] = '\0';

        char hex[20];
        Serial.readBytes(hex, 20);

        uint8_t newCode[10];
        uint8_t *newCodePoint = strToHex(hex);
        for (int i = 0; i < 10; i++)
        {
            newCode[i] = newCodePoint[i];
        }

        myData.easyAdd(newName, newCode);
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
uint8_t *strToHex(char str[])
{
    uint8_t ret[10];
    char part[3];
    for (int i = 0; i < 20; i += 2)
    {
        part[0] = str[i];
        part[1] = str[i + 1];
        part[2] = 0; /* Add terminator */
        ret[i / 2] = strtol(part, NULL, 16);
    }
}