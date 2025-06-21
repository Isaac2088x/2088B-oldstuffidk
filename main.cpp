#include "main.h"
#include "autonomous.hpp" // IWYU pragma: keep
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/widgets/lv_btn.h"
#include "macro.hpp"  // IWYU pragma: keep

using namespace pros;
using namespace std;

void on_center_button() {}

void lvgl_task(void* param) {
    while (true) {
        lv_task_handler(); //handles lvgl tasks
        delay(10);  //super secret maxwell technec 
    }
}

lv_obj_t* screen_main_menu;
lv_obj_t* screen_auton;
lv_obj_t* screen_sensors;

void create_main_menu();
void create_auton_screen();
void create_sensor_screen();

void on_auton_button_clicked(lv_event_t* e) {
    create_auton_screen();      //build screen  
    lv_scr_load(screen_auton);  //switch to it
}

void on_sensor_button_clicked(lv_event_t* e) {
    create_sensor_screen();      //build screen
    lv_scr_load(screen_sensors);  //switch to it
}

void on_back_to_menu(lv_event_t* e) {
    lv_scr_load(screen_main_menu);  // Return to main menu
}

void run_route_1(lv_event_t* e) { //run route 1
    
}

void create_main_menu() {
    screen_main_menu = lv_obj_create(NULL);

    lv_obj_t* title = lv_label_create(screen_main_menu);
    lv_label_set_text(title, "2088B");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    //autons Button
    lv_obj_t* auton_btn = lv_btn_create(screen_main_menu);
    lv_obj_align(auton_btn, LV_ALIGN_CENTER, 0, -30);
    lv_obj_add_event_cb(auton_btn, on_auton_button_clicked, LV_EVENT_CLICKED, NULL);

    lv_obj_t* auton_label = lv_label_create(auton_btn);
    lv_label_set_text(auton_label, "Autonomous");

    //sensor button
    lv_obj_t* sensor_btn = lv_btn_create(screen_main_menu);
    lv_obj_align(sensor_btn, LV_ALIGN_CENTER, 0, 30);
    lv_obj_add_event_cb(sensor_btn, on_sensor_button_clicked, LV_EVENT_CLICKED, NULL);

    lv_obj_t* sensor_label = lv_label_create(sensor_btn);
    lv_label_set_text(sensor_label, "Sensor Readings");
}

void create_auton_screen() {
    screen_auton = lv_obj_create(NULL);

    lv_obj_t* label = lv_label_create(screen_auton);
    lv_label_set_text(label, "Autonomous Routines");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* routine_btn = lv_btn_create(screen_auton);
    lv_obj_align(routine_btn, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t* routine_label = lv_label_create(routine_btn);
    lv_label_set_text(routine_label, "Routine 1");

    lv_obj_t* back_btn = lv_btn_create(screen_auton);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(back_btn, on_back_to_menu, LV_EVENT_CLICKED, NULL);

    lv_obj_t* back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Back");
}

void create_sensor_screen() {
    screen_sensors = lv_obj_create(NULL);

    lv_obj_t* label = lv_label_create(screen_sensors);
    lv_label_set_text(label, "Sensor Readings");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);


    lv_obj_t* back_btn = lv_btn_create(screen_sensors);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(back_btn, on_back_to_menu, LV_EVENT_CLICKED, NULL);

    lv_obj_t* back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Back");
}

std::string alliance = "na";
bool intakeLock = false;

void initializeColourSort() {
  pros::Task([]{
    while (true) {
      if (alliance == "red" && (colour_sensor.get_hue() > 205 && colour_sensor.get_hue() < 230) ||
          (alliance == "blue" && colour_sensor.get_hue() > 0 && colour_sensor.get_hue() < 27)) {
          pros::delay(155);
          intakeLock = true;
          intake.move_voltage(0);
          pros::delay(185);
          intakeLock = false;
          intake.move(12000);
      }

      pros::delay(10);
    }
  });
}


void initialize() {
    pros::Task lvglTask(lvgl_task);
    pros::Task([] {
        chassis.calibrate();
     });

    create_main_menu();           
    lv_scr_load(screen_main_menu);

    imu.reset(false);
    chassis.calibrate(); // calibrate sensors

    colour_sensor.set_led_pwm(100);
    colour_sensor.set_integration_time(10);

    intake.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    initializeColourSort();
    
    alliance = "na";

}

void disabled() {}

void competition_initialize() {}

void autonomous() {

}

void opcontrol() {

    //Bools
    bool clampToggle = false;
    bool doinker1Toggle = false;
    bool liftToggle = false;
    
    std::string states[3] = {"na", "red", "blue"};
    int stateIndex = 0;

	while (true) {

        //lemlib doulbe stick arcade
        int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        chassis.arcade(leftY, rightX);
        
		// arcade control | pros template
		// int dir = master.get_analog(ANALOG_LEFT_Y);    
		// int turn = master.get_analog(ANALOG_RIGHT_X);  
		// left_mg.move(dir - turn);                     
		// right_mg.move(dir + turn);                  

        //      CLAMP    \\

        if(master.get_digital(DIGITAL_R1)){
            clampToggle = !clampToggle;
            clampPiston.set_value(clampToggle);
        }
        //     Doinker     \\

        if(master.get_digital(DIGITAL_R2)){
            doinker1Toggle = !doinker1Toggle;
            doinker.set_value(doinker1Toggle);
        }

        if(master.get_digital(DIGITAL_B)){
            liftToggle = !liftToggle;
            lift.set_value(liftToggle);
        }

        //colour sort toggle

        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            stateIndex++;
            if (stateIndex == 3) {
              stateIndex = 0;
            }
            alliance = states[stateIndex];
          }

        //                        INTAKE                          \\

        if(master.get_digital(DIGITAL_L1)){
            firstStageIntake.move_voltage(12000);
            secondStageIntake.move_voltage(12000);
        } else if (master.get_digital(DIGITAL_L2)) {
            firstStageIntake.move_voltage(-12000);
            secondStageIntake.move_voltage(-12000);
        } else {
            firstStageIntake.move_voltage(0);
            secondStageIntake.move_voltage(0);
        }

		pros::delay(10); //guys trust the super secert maxwell project                  
}
}