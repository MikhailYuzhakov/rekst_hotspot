#include "Printer.h"
#include <HardwareSerial.h>

#define BAUD_RATE 9600

class ConsolePrinter : public Printer
{
    private:
        
    public:
        void init() {
            Serial.begin(BAUD_RATE);
        }

        void println(String message) {
            Serial.println(message);
        }
};
