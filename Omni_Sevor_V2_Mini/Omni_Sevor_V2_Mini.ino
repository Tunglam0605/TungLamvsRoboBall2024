/*
  Omni_Sevor_V2_Mini - Robot omni + servo + cơ cấu bắn
  Tác giả: Tùng Lâm Automatic

  Mô tả:
  - Điều khiển di chuyển bằng PS2.
  - Có servo nâng/hạ bóng, lô bắn, van/xilanh.
  - Có chế độ auto Attack theo timer.

  PS2 (Mega 2560): DAT 28, CMD 26, SEL 24, CLK 22
*/

// Khai báo thư viện.
  #include "PS2X_lib.h"
  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <TungLam_Control_MotorV5.h>
  #include <Servo.h>

// Biến servo.
  Servo servo1;           // Servo nâng bóng.
  uint8_t angle1;
  uint8_t angle_Start = 172; // Góc ban đầu.
  uint8_t angle_End = 100;   // Góc nâng.

// Đối tượng điều khiển motor.
  TungLam_Control_MotorV5 TUNGLAM_Control;

// Chân tay cầm PS2.
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
  bool mode = false;    // Đổi tốc độ di chuyển.
  bool mode1 = false;
  bool isMoving = false;
  bool L2_State = false;
  uint8_t speed;
  uint8_t speed6;
  uint8_t speed2 = 100; // Tốc độ lô bắn.
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
  unsigned long timer1, timerz; // Timer cho auto.
  bool Auto = 0;
  uint8_t Buoc = 0;             // Bước auto.
  bool xoay = 0;                // Cờ xoay 180 độ.
  unsigned long timerx, timer2;

// Setup.
  void setup()
    {
      Serial.begin(115200);

      // Khởi tạo điều khiển motor.
      TUNGLAM_Control.Mode1();

      // Servo: gắn vào chân D9.
      servo1.attach(9);
      servo1.write(angle_Start);

      // PWM cho DC hút/bắn và chân phụ PB7.
      DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB7);
      PORTB &= ~(1 << PB5);
      PORTB &= ~(1 << PB4);
      PORTB &= ~(1 << PB7);

      // Chân van/xilanh và ghim.
      DDRL |= (1 << PL5);
      PORTL &= ~(1 << PL5);
      DDRD |= (1 << PD7);
      PORTD &= ~(1 << PD7);

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
      Button();
      Joystick();

      // Auto Attack trong 3.75s.
      if (millis() - timer1 < 3750 && Auto == 1) Attack();
      if (millis() - timer1 > 3750 && millis() - timer1 < 3850 && Auto == 1) Auto = 0;

      // Xoay 180 độ theo timer.
      if (millis() - timerx < 690 && xoay) {
        TUNGLAM_Control.moveLeft(255);
      }
      if (millis() - timerx > 690 && millis() - timerx < 720) {
        xoay = 0;
      }
    }
