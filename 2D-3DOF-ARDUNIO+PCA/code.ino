#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create a PCA9685 object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define servo channels on the PCA9685
#define SHOULDER_SERVO 0
#define ELBOW_SERVO 1
#define WRIST_SERVO 2

// Link lengths .its in bloddy "cm"
const float L1 = 35.0;  // Length of the first link
const float L2 = 22.0;  // Length of the second link
const float L3 = 20.0;  // Length of the end effector

// Input variables (change these values directly)
float target_x = 50.0;     // Target x-coordinate
float target_z = 40.0;     // Target z-coordinate
float desired_orientation = 0.0; // Desired wrist orientation in degrees

// Previous input values for tracking changes
float prev_target_x = 0.0;
float prev_target_z = 0.0;
float prev_orientation = 0.0;

// Servo configuration
#define SERVO_MIN 150  // Min pulse length out of 4096
#define SERVO_MAX 600  // Max pulse length out of 4096

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Set frequency to 50 Hz (for servos)

  Serial.println("Robotic arm initialized.");
}

void loop() {
  // Check if inputs have changed
  if (target_x != prev_target_x || target_z != prev_target_z || desired_orientation != prev_orientation) {
    // Move the arm to the new position
    moveToTarget(target_x, target_z, desired_orientation);

    // Update previous values
    prev_target_x = target_x;
    prev_target_z = target_z;
    prev_orientation = desired_orientation;
  }

  delay(100); // Small delay to prevent rapid updates
}

// Function to calculate and move to the target position
void moveToTarget(float x, float z, float orientation) {
  // Calculate effective radius and z-position
  float r_eff = sqrt(pow(x, 2) + pow(z, 2)) - L3;
  float z_eff = z;

  // Check reachability
  if (r_eff < 0 || sqrt(pow(r_eff, 2) + pow(z_eff, 2)) > (L1 + L2)) {
    Serial.println("Target is not reachable.");
    return;
  }

  // Calculate elbow angle (theta2)
  float cos_theta2 = (pow(r_eff, 2) + pow(z_eff, 2) - pow(L1, 2) - pow(L2, 2)) / (2 * L1 * L2);
  float theta2 = acos(cos_theta2);

  // Calculate shoulder angle (theta1)
  float theta1 = atan2(z_eff, r_eff) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));

  // Calculate wrist angle (theta3)
  float theta3 = radians(orientation) - (theta1 + theta2);

  // Convert to degrees
  float theta1_deg = degrees(theta1);
  float theta2_deg = degrees(theta2);
  float theta3_deg = degrees(theta3);

  // Print calculated angles
  Serial.print("Shoulder (theta1): ");
  Serial.print(theta1_deg);
  Serial.println(" degrees");

  Serial.print("Elbow (theta2): ");
  Serial.print(theta2_deg);
  Serial.println(" degrees");

  Serial.print("Wrist (theta3): ");
  Serial.print(theta3_deg);
  Serial.println(" degrees");

  // Map angles to PCA9685 servo pulse width
  int shoulderPulse = map(constrain(theta1_deg, -90, 90), -90, 90, SERVO_MIN, SERVO_MAX);
  int elbowPulse = map(constrain(theta2_deg, 0, 180), 0, 180, SERVO_MIN, SERVO_MAX);
  int wristPulse = map(constrain(theta3_deg, -90, 90), -90, 90, SERVO_MIN, SERVO_MAX);

  // Move servos to calculated positions
  pwm.setPWM(SHOULDER_SERVO, 0, shoulderPulse);
  pwm.setPWM(ELBOW_SERVO, 0, elbowPulse);
  pwm.setPWM(WRIST_SERVO, 0, wristPulse);

  Serial.println("Servos moved to target position.");
}
