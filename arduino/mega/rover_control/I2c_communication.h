#ifndef I2C_COMMUNICATION_H
#define I2C_COMMUNICATION_H

void i2cSetup();
void processCoordinates();
bool isNavigating();
void processNavigation();
void receiveData(int byteCount);

#endif
