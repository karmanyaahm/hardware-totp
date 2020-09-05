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

class data
{
private:
    Service keys[15];
    short int max = -1; //biggest that is defined
    byte currScreen = 0;
    TOTP totp = TOTP((byte *)serv.code, 10, 30);

public:
    Service *getKeys()
    {
        return keys;
    }
    data() {}
    data(Service k[])
    {
        for (int i = 0; i < 15; i++)
        {
            keys[i] = k[i];
        }
    }

    char *name;
    void init()
    {
        uint8_t newCode[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
        char newName[] = "test123";
        Service neww;
        neww.set(newName, newCode);
        addKey(neww);
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
};

data myData;
