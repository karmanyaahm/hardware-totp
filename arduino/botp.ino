#include "TOTP.h"

class Service
{
public:
    char name[11] = "          ";
    uint8_t code[10] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
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
Service keys[15];

class data
{
private:
    byte currScreen = 0;
    TOTP totp = TOTP((byte *)serv.code, 10, 30);

public:
    short int max = -1; //biggest that is defined
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
        name = tmp->name;
        Serial.println(name);

        return currScreen;
    }
    int addKey(Service s)
    {
        if (max >= 15)
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
    char *getCode(long now)
    {
        return totp.getCode(now);
    }
    void remove (int i)
    {
        for (i; i < max; i++)
        {
            keys[i] = keys[i + 1];
        }
        max--;
    }
};

data myData;
