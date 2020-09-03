
void setup()
{
    Serial.begin(SERIAL_SPEED);

    delay(1000);
    Serial.println("setup");
}
timer myTimer;

void loop()
{
    Serial.println('h');
    // time_t now = myTimer.getTime();

    // Serial.println(now);

    // delay(1000);
}
