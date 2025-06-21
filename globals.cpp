#include "main.h" // IWYU pragma: keep

using namespace pros;

Controller master(pros::E_CONTROLLER_MASTER); 

//                                                           Motor Declarations                                                                \\

MotorGroup left_motor_group({-1, 2, 3}, pros::MotorGears::blue);
MotorGroup right_motor_group({-4, -5, 6}, pros::MotorGears::blue);

Motor firstStageIntake(12);
Motor secondStageIntake(-11);

MotorGroup intake({12, -11});

Motor ladybrown( 9);
Rotation wallstakeRot(19);

//                                                        C++ TRI-PORT Declarations                                                             \\

adi::DigitalOut clampPiston('A');
adi::DigitalOut doinker('B');
adi::DigitalOut lift('C');

Optical colour_sensor(16);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          LEMLIB DECLARIATIONs                                                                \\

// drivetrain settings
lemlib::Drivetrain DT(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450, // drivetrain rpm is 360
                              2 // horizontal drift is 2 
);

Imu imu(10);
Rotation horizontal_encoder(8);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_325, -5.75);

// odometry settings
lemlib::OdomSensors sensors(nullptr,
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
    10, // minimum output where drivetrain will move out of 127
    1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
 10, // minimum output where drivetrain will move out of 127
 1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(DT,
lateral_controller,
angular_controller,
sensors,
&throttle_curve, 
&steer_curve
);