#ifndef I2C_COMMUNICATION_H
#define I2C_COMMUNICATION_H

#include <Arduino.h>  // Include Arduino core for String and Serial

void i2cSetup();
void processCoordinates();
bool isNavigating();
char i2cReadCommand();
void processNavigation();
void receiveData(int byteCount);
void clearCoordsArray();
void parseMaxIndex(String data);
void parseAndStoreCoordinates(String data);
void parseDiameterAndFrequency(String data);

#endif
