#include "TOTP.h"

class Service
{
public:
    char name[11] = "          ";
    uint8_t code[11] = {0x0, 0xC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    void set(char newName[], uint8_t newCode[])
    {
        strncpy(name, newName, sizeof(name) - 1);
        strncpy(code, newCode, sizeof(code));
    }
    Service()
    {
    }
};
Service serv;
static Service keys[HOW_MANY_OTP];

class data
{
private:
    byte currScreen = 0;
    TOTP totp = TOTP((byte *)serv.code, 10, 30);

public:
    int8_t max = -1; //biggest that is defined
    data() {}

    char *name;
    void init()
    {

        toScreen(0);
    }
    int toScreen()
    {
        // next screen if no argument
        currScreen++;
        currScreen %= max + 1;
        toScreen(currScreen);
    }
    int toScreen(int num)
    {
        Serial.println(keys[currScreen].name);

        if (num > max)
            return -1;
        currScreen = num;
        Service *tmp = &(keys[currScreen]);
        totp = TOTP(tmp->code, 10, 30);
        char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

        for (int i = 0; i < 10; ++i)
        {
            char const byte = (tmp->code)[i];

            Serial.print(hex_chars[(byte & 0xF0) >> 4]);
            Serial.print(hex_chars[(byte & 0x0F) >> 0]);
        }
        for (int i = 0; i < 10; ++i)
        {
            char const byte = keys[1].code[i];

            Serial.print(hex_chars[(byte & 0xF0) >> 4]);
            Serial.print(hex_chars[(byte & 0x0F) >> 0]);
        }
        name = tmp->name;
        Serial.println(name);

        return currScreen;
    }
    int addKey(Service s)
    {
        if (max >= HOW_MANY_OTP)
            return -1;
        max++;
        keys[max].set(s.name, s.code);
    }
    void easyAdd(char newName[], uint8_t newCode[])
    {

        Service neww;
        neww.set(newName, newCode);
        addKey(neww);
    }
    void getCode(char *buf, unsigned long now)
    {
        if (currScreen == 0)
        {
            uint8_t hour = (now % 86400) / 3600;
            uint8_t minute = (now % 3600) / 60;
            sprintf(buf, "%02d:%02d ", hour, minute);
            //Serial.println(buf);
        }
        else
        {
            char *t = totp.getCode(now);
            strcpy(buf, t);
        }
    }
    void remove(int i)
    {
        for (i; i < max; i++)
        {
            keys[i] = keys[i + 1];
        }
        max--;
    }
};

data myData;
