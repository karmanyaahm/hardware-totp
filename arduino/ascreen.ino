
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class screen
{
private:
public:
    screen(){};
    void init();
    void homeScreen(String str, char *code);
    void lineUpdate(double interval);
    void off();
};
void screen::init()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    homeScreen("homescreen", (char *)"homescreen");
    delay(500);
}
void screen::off()
{
    display.clearDisplay();
    display.display();
}
void screen ::homeScreen(String str, char *code)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.println(str);
    display.println();
    display.setTextSize(2); // Draw 2X-scale text
    display.println(code);
    display.display();
}
void screen ::lineUpdate(double interval)
{
    display.drawFastHLine(0, display.height() - 1, (display.width() - 1), SSD1306_BLACK);
    display.drawFastHLine(0, display.height() - 1, (1 - (interval / 30)) * (display.width() - 1), SSD1306_WHITE);
    display.display();
}
screen myScreen;