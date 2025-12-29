/*
  RoboBall_STEP - Robot mecanum + STEP nâng/hạ + servo
  Tác giả: Tùng Lâm Automatic

  Mô tả:
  - Điều khiển bằng PS2.
  - Cơ cấu STEP nâng/hạ bóng, servo hỗ trợ.

  Sơ đồ chân chính (Mega 2560):
  - PS2: DAT 28, CMD 26, SEL 24, CLK 22
  - Servo: D9
  - STEP1: PUL = PE4 (D2), DIR = PE5 (D3), ENA = PL7 (D42)
  - CTHT STEP1: D18, D19
  - DC hút/bắn: PB5 (D11), PB4 (D10)
*/

// Thư viện.
  #include "PS2X_lib.h"
  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <TungLam_Control_MotorV5.h>
  #include <Servo.h>

// Biến servo.
  Servo servo1;
  uint8_t angle1;
  uint8_t angle_Start = 180;
  uint8_t angle_End = 0;

// Đối tượng điều khiển motor.
  TungLam_Control_MotorV5 TUNGLAM_Control;

// Chân PS2.
  #define PS2_DAT 28
  #define PS2_CMD 26
  #define PS2_SEL 24
  #define PS2_CLK 22

// Biến tay cầm.
  int LX, LY, RX, RY;
  int LX2, LY2, RX2, RY2;
  PS2X ps2x;
  int error = 0;
  byte type = 0;
  byte vibrate = 0;

// Biến di chuyển.
  uint8_t State_DC10 = 0;
  uint8_t pre;
  bool mode = false;
  bool mode1 = false;
  bool isMoving = false;
  bool L2_State = false;
  uint8_t speed;
  uint8_t speed6;
  uint8_t speed2 = 100;
  uint8_t speed1 = mode1 ? 0 : 255;
  uint8_t Speed_Auto = 255;
  int Speed_Auto1 = 0;
  uint8_t TIM = 10;
  unsigned long startTime = 0;
  bool Set = true;

// Biến cơ cấu phụ.
  bool DC_H  = false;
  bool DC_B  = false;
  bool control = false;
  bool ModeZ = 0;

// CTHT STEP1.
  #define LIMIT_SWITCH1_STEP1 18
  #define LIMIT_SWITCH2_STEP1 19

// Biến STEP.
  int stepsPerRev = 100;
  int delayTime = 500;
  int pauseTime = 500;
  unsigned long key;

  volatile bool stopForward1 = false;
  volatile bool stopBackward1 = false;
  volatile bool stopForward2 = false;
  volatile bool stopBackward2 = false;

// Setup.
  void setup()
    {
      Serial.begin(115200);

      // Khởi tạo điều khiển motor.
      TUNGLAM_Control.Mode1();

      // PWM cho DC hút/bắn.
      DDRB |= (1 << PB5) | (1 << PB4);
      PORTB &= ~(1 << PB5);
      PORTB &= ~(1 << PB4);

      // STEP1: PUL/DIR/ENA.
      DDRE |= (1 << PE4) | (1 << PE5);
      DDRL |= (1 << PL7);
      PORTL |= (1 << PL7); // Enable.

      // CTHT STEP1.
      pinMode(LIMIT_SWITCH1_STEP1, INPUT_PULLUP);
      pinMode(LIMIT_SWITCH2_STEP1, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH1_STEP1), stopForwardInterrupt1, FALLING);
      attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH2_STEP1), stopBackwardInterrupt1, FALLING);

      // Cấu hình tay cầm PS2.
      Serial.print("Search Controller..");
      do {
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
        if (error == 0) {
          Serial.println("\nConfigured successful ");
          break;
        } else {
          Serial.println("No");
          TUNGLAM_Control.STOP();
          delay(100);
        }
      } while (1);

      type = ps2x.readType();
      switch (type) {
        case 0: Serial.println("Unknown Controller type found "); break;
        case 1: Serial.println("DualShock Controller found "); break;
        case 2: Serial.println("GuitarHero Controller found "); break;
        case 3: Serial.println("Wireless Sony DualShock Controller found "); break;
      }

      ps2x.read_gamepad(true, 200);
      delay(200);
      ps2x.read_gamepad(false, 200);
      delay(300);
      ps2x.read_gamepad(true, 200);
      delay(200);
    }

// Loop.
  void loop()
    {
      ps2x.read_gamepad(false, vibrate);
      Butom();
      Joystick();
    }
