#include <Arduino.h>
#include <TinyWireM.h>
#include <SoftwareSerial.h>
#include <SHTSensor.h>
#include "SimpleModbusSlaveSoftwareSerial.h"

#define RX 4           // Arduino defined pin (PB0, package pin #5), used for serial RX from MAX485
#define TX 1           // Arduino defined pin (PB1, package pin #6), used to TX to MAX485
#define RS485_EN 3     // pin to set transmission mode on MAR485  (PB2, package pin #7)
#define BAUD_RATE 9600 // baud rate for serial communication

// Modbus device ID is defined as a platform io environment define please check platformio.ini

enum
{
    // The first register starts at address 0
    TEMP,
    RH,
    TOTAL_ERRORS,
    // leave this one
    TOTAL_REGS_SIZE
    // total number of registers for function 3 and 16 share the same register array
};

unsigned int holdingRegs[TOTAL_REGS_SIZE]; // function 3 and 16 register array

SHTSensor sht(SHTSensor::SHT4X);
SoftwareSerial serial(RX, TX);

unsigned long previousMillis = 0; // This stores the millis() value when last sensor reading was done.

const long interval = 1000; // Time interval between sensor readings.

void SHTRead()
{

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {

        previousMillis = currentMillis;

        if (sht.readSample())
        {
            holdingRegs[RH] = int(sht.getHumidity() * 10);
            holdingRegs[TEMP] = int(sht.getTemperature() * 10);
        }
        else
        {
            holdingRegs[RH] = -100;
            holdingRegs[TEMP] = -100;
        }
    }
}

void modbus()
{
    holdingRegs[TOTAL_ERRORS] = modbus_update(holdingRegs);
    delayMicroseconds(50);
}

void setup()
{
    modbus_configure(&serial, BAUD_RATE, DEVICE_ID, RS485_EN, TOTAL_REGS_SIZE);
    TinyWireM.begin(); // initialize I2C lib

    sht.init();
}

void loop()
{
    SHTRead();
    modbus(); 
}
