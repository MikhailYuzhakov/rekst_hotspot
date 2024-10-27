#include <DS3232RTC.h> //для работы с часами реального времени DS3231
#include <Wire.h> //для работы с шиной I2C (TWI)
#include <GyverNTP.h>

#define NTP_REFRESH_PERIOD 60000

class RealTimeClock
{
    private:
        uint32_t timer;
    public:
        bool init(int8_t timezone) {
            NTP.setPeriod(10);
            return NTP.begin(timezone);
        }

        void ntpClientHandler() {
            NTP.tick();
            if (millis() - timer >= NTP_REFRESH_PERIOD) {
                timer = millis();
                Serial.println(NTP.toString());
            }
        }
};