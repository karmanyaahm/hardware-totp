#include <EEPROM.h>

#define BUTTON_PIN 2
volatile bool pressed = false;

void press()
{
    Serial.println("press");
    pressed = true;
}

void eepromStore()
{
    EEPROM.put(0, myData.max);
    Serial.println(myData.max);

    for (int i = 0; (i < 15) && (i <= myData.max); i++)
    {
        EEPROM.put(16 + (40 * i), keys[i]);
    }
}
void eepromRead()
{
    // Serial.println("eep");
    short int max;
    EEPROM.get(0, max);

    for (int i = 0; (i < 15) && (i <= max); i++)
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
