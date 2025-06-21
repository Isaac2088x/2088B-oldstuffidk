#include "lemlib/api.hpp"  // IWYU pragma: keep
#include "pros/motor_group.hpp"

using namespace pros;

extern Controller master; 

//                                                           Motor Declarations                                                                \\

extern MotorGroup left_motor_group;
extern MotorGroup right_motor_group;

extern Motor firstStageIntake;
extern Motor secondStageIntake;
extern MotorGroup intake;

extern Motor ladybrown;

extern Optical colour_sensor; //clangd issue idk how to fix

//                                                        C++ TRI-PORT Declarations                                                             \\

extern adi::DigitalOut clampPiston;
extern adi::DigitalOut doinker;
extern adi::DigitalOut lift;

//                                                                Sensors                                                                       \\

extern Imu imu;
extern Rotation horizontal_encoder;
extern Rotation wallstakeRot;

//                                                          LEMLIB DECLARIATIONs                                                                \\

extern lemlib::Chassis chassis;