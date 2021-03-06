
const char TIME_HEADER = 'T';
const char GET_TIME_HEADER = 'G';
const char ADD_HEADER = 'A';
const char LIST_HEADER = 'L';
const char REMOVE_HEADER = 'R';
const char SAVE_HEADER = 'S';
const char OUT_SEPERATOR = ';';
const char IN_SEPERATOR = ':';

void processSyncMessage()
{

    char b = char(Serial.read());
    if (b == TIME_HEADER)
    {
        unsigned long pctime;
        pctime = Serial.parseInt();
        if (pctime >= MIN_TIME)
        { // check the integer is a valid time (greater than Jan 1 2013)
            myTimer.setTime(pctime);
            delay(300);
        }
    }
    else if (b == GET_TIME_HEADER)
    {
        Serial.println(myTimer.getTime());
    }
    else if (b == LIST_HEADER)
    {
        for (uint8_t i = 0; i <= myData.max; i++)
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
        int8_t a = Serial.parseInt();
        if (a == -69)
        {

            myData = data();
        }
        else
        {
            myData.remove(a);
        }
    }
    else if (b == ADD_HEADER)
    {
        uint8_t len = Serial.parseInt(SKIP_NONE);
        Serial.read();
        char newName[len];

        uint8_t a = Serial.readBytesUntil(IN_SEPERATOR, newName, len + 2);
        newName[a] = '\0';

        char hex[len * 2];
        Serial.readBytes(hex, len * 2);
        uint8_t newCode[len];
        strToHex(newCode, hex, len);

        myData.easyAdd(newName, newCode, len);
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
void strToHex(uint8_t *arr, char str[], uint8_t len)
{
    char part[3];
    for (uint8_t i = 0; i < len * 2; i += 2)
    {
        part[0] = str[i];
        part[1] = str[i + 1];
        part[2] = 0; /* Add terminator */
        arr[i / 2] = strtol(part, NULL, 16);
    }
}