#include <EEPROM.h>

const byte BUTTON_PIN = 2;
volatile bool pressed = false;

void press()
{
    Serial.println(F("press"));
    pressed = true;
}

void eepromStore()
{
    EEPROM.put(0, myData.max);
    Serial.println(myData.max);

    for (int i = 0; (i < HOW_MANY_OTP) && (i <= myData.max); i++)
    {
        EEPROM.put(16 + (40 * i), keys[i]);
    }
}
void eepromRead()
{
    // Serial.println("eep");
    short int max;
    EEPROM.get(0, max);

    for (int i = 0; (i < HOW_MANY_OTP) && (i <= max); i++)
    {
        Service serv;
        EEPROM.get(16 + (40 * i), serv);
        myData.easyAdd(serv.name, serv.code);
        //     Serial.println(serv.name);
        //     Serial.println((i >= myData.max));
        //     Serial.println(myData.max);
        //     Serial.println("==");
    }
    // Serial.println("eep");
}
char code[7];
