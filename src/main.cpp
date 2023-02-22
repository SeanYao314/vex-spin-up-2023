#include "main.h"


// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-12,-2}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{-19, 10}

  // IMU Port
  ,20

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.75

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.25


  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);
Drive ptochassis {
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-12,-2,13}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{-17, 19, 10}

  // IMU Port
  ,20

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.75

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.33333333


  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
};
Drive ptochassis2 {
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-12,-2,11,3}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{19, 10,-17, 9}

  // IMU Port
  ,20

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.75

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.3333333


  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
};

const int CATA_LEFT  = 3;
const int CATA_RIGHT   = 9;

const int CATA_BACK_LEFT = 13;
const int CATA_BACK_RIGHT = 17;

pros::Motor cata_right(CATA_RIGHT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata_left(CATA_LEFT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata_back_right(CATA_BACK_RIGHT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata_back_left(CATA_BACK_LEFT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::ADIDigitalOut ptoMain('A');
pros::ADIDigitalOut ptoSecondary('B');
pros::ADIDigitalOut changer('F');
pros::ADIDigitalOut endgame('E');
pros::ADIDigitalOut intakeMover('C');

pros::ADIDigitalIn limitSwitch('G');


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::print_ez_template();
  
  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
  // chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
    Auton("Example Drive\n\nDrive forward and come back.", drive_example),
    Auton("Example Turn\n\nTurn 3 times.", turn_example),
    Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
    Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
    Auton("Swing Example\n\nSwing, drive, swing.", swing_example),
    Auton("Combine all 3 movements", combining_movements),
    Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}



/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}



/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

  // ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector

}



/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */



void cata_drive(int speed) {
  cata_left.move_voltage(-speed);
  cata_right.move_voltage(speed);
}
void cata_drive_secondary(int speed) {
  cata_back_left.move_voltage(-speed);
  cata_back_right.move_voltage(-speed);
}


void intake_move_drive(bool intake) {
  intakeMover.set_value(intake);
}

void changer_drive(bool change) {
  changer.set_value(change);
}


void opcontrol() {
  int cataPos;
  int ptoCurrent = 4;
  while(true) {

    cataPos = limitSwitch.get_value();
    if(ptoCurrent == 4 ) {
      chassis.tank(); //automatically start out with 4m chassis / 4m intake and 4m cata, maybe change idk -- this is what we run during auton and skills(probably) because we don't have to deal with defence, also what we run while no one is playing defence on us. more powerful intake and cata but 66% power of 6m drive
    } else if(ptoCurrent == 6) {
      ptochassis.tank(); //this is 6m drive and 2m cata/intake, used during majority of comp operator control games. drive is 50% more powerful than 4m drive however intake and cata power is halved -- this isn't that big of a deal since we don't max out the intake and cata anyways so cata/intake performance probably decreases 10-20%
    } else if (ptoCurrent == 8) { // this is for pushing power, but is really slow. you can't use cata or intake at all or rollers, but chassis is up to twice the pushing power of 6m drive and 3 times the pusing power of 4m drive. used rarely, mostly defending rollers. downside is that you move 3 times slower - which is why we only use in pushing battles where you never move at full speed anyways
      int r = master.get_analog(ANALOG_LEFT_Y);
      int l = master.get_analog(ANALOG_RIGHT_Y);

      float pushR = (600-cata_right.get_actual_velocity())/3;
      float pushL = (600-cata_left.get_actual_velocity())/3;
      ptochassis.joy_thresh_opcontrol(r/3+pow(2.718281828,pushR*0.032),l/3+pow(2.718281828,pushL*0.032)); //power function added to 200rpm chassis, more pushing power. dangerous b/c chassis motors are spinning different speed, its fine when you are in a pushing battle w someone and moving slower than 200 rpm
      
      cata_right.move(r*600/127);
      cata_left.move(l*600/127);
    }

    if(!master.get_digital(DIGITAL_L1)) {
      cata_drive((ptoCurrent==4||ptoCurrent==6)*12000*(-cataPos+1+master.get_digital(DIGITAL_R1)));
      cata_drive_secondary((ptoCurrent==4)*12000*(-cataPos+1+master.get_digital(DIGITAL_R1)));  //cata control -- cata moves back when limit switch is not activated or when user inputs R1
    }

    if(!master.get_digital(DIGITAL_R1)&&cataPos) {
      cata_drive((ptoCurrent==4||ptoCurrent==6)*-12000*master.get_digital(DIGITAL_L1));
      cata_drive_secondary((ptoCurrent==4)*-12000*master.get_digital(DIGITAL_L1)); //while the cata is not moving, intake moves when user inputs L1
    }


    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
