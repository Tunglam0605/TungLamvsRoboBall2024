/*
  Meganum_ButomMove - Điều khiển robot mecanum bằng nút PS2
  Tác giả: Tùng Lâm Automatic

  Mô tả:
  - Điều khiển di chuyển bằng các nút D-pad và nút quay.
  - Không dùng joystick, phù hợp test nhanh.

  PS2 (Mega 2560): DAT 28, CMD 26, SEL 24, CLK 22
*/

// Khai báo thư viện.
  #include "PS2X_lib.h"
  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <TungLam_Control_MotorV5.h>

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
  int i = 1; // Biến phụ.

// Biến di chuyển.
  uint8_t State_DC10 = 0;
  uint8_t pre;
  bool mode = false;    // Đổi tốc độ di chuyển.
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

// Biến điều khiển cơ cấu phụ (chưa dùng).
  bool DC_H  = false;
  bool DC_B  = false;
  bool control = false;
  bool ModeZ = 0;

// CTHT STEP (không dùng trong file này).
  #define LIMIT_SWITCH1_STEP1 18
  #define LIMIT_SWITCH2_STEP1 19

// Biến STEP (không dùng trong file này).
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
      TUNGLAM_Control.setTimABS(10, 20, 20, 20, 20, 25);

      // Chân phụ PL5 (dự phòng).
      DDRL |= (1 << PL5);
      PORTL &= ~(1 << PL5);

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
    }

// Xử lý nút bấm.
  void Butom()
    {
      bool buttonHandled = false;

      // Điều khiển di chuyển bằng D-pad.
      if (ps2x.Button(PSB_PAD_UP)) {
        Serial.println("Nhấn nút PAD_UP");
        TUNGLAM_Control.moveForward(speed);
        buttonHandled = true;
      }
      if (ps2x.Button(PSB_PAD_DOWN)) {
        Serial.println("Nhấn nút PAD_DOWN");
        TUNGLAM_Control.moveBackward(speed);
        buttonHandled = true;
      }
      if (ps2x.Button(PSB_PAD_LEFT)) {
        Serial.println("Nhấn nút PAD_LEFT");
        TUNGLAM_Control.moveLeftSide(speed);
        buttonHandled = true;
      }
      if (ps2x.Button(PSB_PAD_RIGHT)) {
        Serial.println("Nhấn nút PAD_RIGHT");
        TUNGLAM_Control.moveRightSide(speed);
        buttonHandled = true;
      }

      // Điều khiển quay bằng SQUARE/CIRCLE.
      if (ps2x.Button(PSB_SQUARE)) {
        Serial.println("Nhấn nút SQUARE");
        TUNGLAM_Control.moveRight(speed6);
        buttonHandled = true;
      }
      if (ps2x.Button(PSB_CIRCLE)) {
        Serial.println("Nhấn nút CIRCLE");
        TUNGLAM_Control.moveLeft(speed6);
        buttonHandled = true;
      }

      // TRIANGLE/CROSS giữ lại để test nhanh.
      if (ps2x.Button(PSB_TRIANGLE)) {
        Serial.println("Nhấn nút TRIANGLE");
        TUNGLAM_Control.moveForward(speed);
        buttonHandled = true;
      }
      if (ps2x.Button(PSB_CROSS)) {
        Serial.println("Nhấn nút CROSS");
        TUNGLAM_Control.moveBackward(speed);
        buttonHandled = true;
      }

      // L1: đổi mode tốc độ.
      if (ps2x.ButtonPressed(PSB_L1)) {
        Serial.println("Nhấn nút L1 - đổi tốc độ");
        mode = !mode;
        if (mode) {
          speed = 255;
          speed6 = 200;
        } else {
          speed = 200;
          speed6 = 180;
        }
        Serial.print("Chế độ tốc độ: ");
        Serial.println(mode ? "Nhanh" : "Chậm");
        buttonHandled = true;
      }

      // Nếu không nhấn nút nào -> ABS để dừng.
      if (!buttonHandled) {
        TUNGLAM_Control.ABS(speed);
      }
      delay(20);
    }
