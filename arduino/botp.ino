#include "TOTP.h"

class Service
{
public:
    char name[11] = "          \0";
    uint8_t code[MAX_CODE_SIZE] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    uint8_t len = 1;
    void set(char newName[], uint8_t newCode[], uint8_t lent)
    {
        strncpy(name, newName, sizeof(name) - 1);
        strncpy(code, newCode, sizeof(code));
        len = lent;
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
    TOTP totp = TOTP((byte *)serv.code, serv.len, 30);

public:
    byte currScreen = 0;

    int8_t max = -1; //biggest that is defined
    data() {}

    char *name;
    void init()
    {

        toScreen(0);
    }
    uint8_t toScreen()
    {
        // next screen if no argument
        currScreen++;
        currScreen %= max + 1;
        return toScreen(currScreen);

    }
    uint8_t toScreen(uint8_t num)
    {

        if (num > max)
            return -1;
        currScreen = num;
        Service *tmp = &(keys[currScreen]);
        totp = TOTP((*tmp).code, (*tmp).len, 30);

        // char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

        // for (uint8_t i = 0; i < (*tmp).len; ++i)
        // {
        //     char const byte = ((*tmp).code)[i];

        //     Serial.print(hex_chars[(byte & 0xF0) >> 4]);
        //     Serial.print(hex_chars[(byte & 0x0F) >> 0]);
        // }
        name = (*tmp).name;
        Serial.println(name);
        return currScreen;
    }
    uint8_t addKey(Service s)
    {
        if (max >= HOW_MANY_OTP)
            return -1;
        max++;
        keys[max].set(s.name, s.code, s.len);
        return max;
    }
    void easyAdd(char newName[], uint8_t newCode[], uint8_t len)
    {

        Service neww;
        neww.set(newName, newCode, len);
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
    void remove(uint8_t i)
    {
        for (i; i < max; i++)
        {
            keys[i] = keys[i + 1];
        }
        max--;
    }
};

data myData;
