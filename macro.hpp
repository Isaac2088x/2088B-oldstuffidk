#pragma once
#include "main.h"

extern int wallstakeStates[3] = {75, 91, 230}; 
// [0] = arm is below hooks (starting position)
// [1] = loading position
// [2] = scoring position
extern int currState = 0;

int kP = 2.6; 
int kI = 0; 
int kD = 1;

inline void liftControl(double target) {
    double error = 0;
    double integral = 0;
    double derivative = 0;
    double prevError = 0;

    error = target - wallstakeRot.get_position() / 100.0;

    double timer = 0;

    ladybrown.move(80);
    while (timer < 800) { 
        error = target - wallstakeRot.get_position() / 100.0;
        integral += error;
        derivative = prevError - error;

        ladybrown.move(error * kP + integral * kI + derivative * kD);

        if (fabs(error) < 1) {
            break;
        }

        timer += 10;
        prevError = error;
        pros::delay(10);
    }
    ladybrown.brake();
}