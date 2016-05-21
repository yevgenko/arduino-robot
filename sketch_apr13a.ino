// Moto Lib
#include <AFMotor.h>

// define motos
AF_DCMotor leftMoto(2);
AF_DCMotor rightMoto(1);

// moto directions per robot construction
#define FWD BACKWARD
#define BWD FORWARD
#define REL RELEASE

#define MAX_SPEED 255
#define LOW_SPEED 220

// Light Sensors Pins
const int LANE_PINS[] = {A5, A4, A3, A2};

// Light Sensors States
#define LANE_LOST  0 // 0000
/*#define LANE_LEFT 8 // 1000 - the left sensonr is broken*/
#define LANE_LEFT  4 // 0100
#define LANE_RIGHT 1 // 0001

/**
 * Helper Functions
 */

/**
 * Light Sensors
 */

int lightSensorsState() {
  int state = 0; // 0000

  // cycle thru the pins
  for (int i = 0; i < 4; i++) {
    state <<= 1;
    state |= readLaneBit(LANE_PINS[i]);
  }

  return state;
}

int readLaneBit(int pin) {
  return digitalRead(pin) == HIGH ? 1 : 0;
}

boolean isLineLost() {
  return lightSensorsState() == LANE_LOST;
}

boolean isLineUnderLeftSide() {
  return lightSensorsState() == LANE_LEFT;
}

boolean isLineUnderRightSide() {
  return lightSensorsState() == LANE_RIGHT;
}

boolean isLineMid() {
  return lightSensorsState() == LANE_MID;
}

void laneStateLogger() {
  Serial.print("IR Lane: ");
  int state = lightSensorsState();
  for (int i = 3; i >= 0; i--) {
    Serial.print(bitRead(state, i));
  }
  Serial.println();
}

/**
 * Motors
 */

void moveForward() {
  leftMoto.run(FWD);
  rightMoto.run(FWD);
}

void stop() {
  leftMoto.run(REL);
  rightMoto.run(REL);
}

void moveRight() {
  leftMoto.run(FWD);
  rightMoto.run(REL);
}

void moveLeft() {
  leftMoto.run(REL);
  rightMoto.run(FWD);
}

void rotate() {
  leftMoto.run(BWD);
  rightMoto.run(FWD);
}

void lowSpeed() {
  leftMoto.setSpeed(LOW_SPEED);
  rightMoto.setSpeed(LOW_SPEED);
}

void maxSpeed() {
  leftMoto.setSpeed(MAX_SPEED);
  rightMoto.setSpeed(MAX_SPEED);
}

/**
 * Main
 */

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(LANE_PINS[i], INPUT);
  }

  // init on motors
  maxSpeed();
  stop();
}

void loop() {
  laneStateLogger();

  if (isLineLost()) {
    Serial.print("Lost Line, searching...");
    lowSpeed();
    rotate();
  } else if (isLineUnderLeftSide()) {
    Serial.print("Turning Left...");
    moveLeft();
  } else if (isLineUnderRightSide()) {
    Serial.print("Turning Right...");
    moveRight();
  } else {
    Serial.print("Moving...");
    maxSpeed();
    moveForward();
  }

  Serial.println();
}
