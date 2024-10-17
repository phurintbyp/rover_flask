#ifndef PID_CONTROL_H
#define PID_CONTROL_H

void pidSetup();
void updatePID();
void updateTargetCoordinates(double target_lat, double target_lon);
void adjustMotorSpeeds(double correction);

#endif
