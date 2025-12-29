/*
  Thumon_Meganum - Thủ môn mecanum điều khiển PS2
  Tác giả: Tùng Lâm Automatic

  Mô tả:
  - Điều khiển di chuyển bằng PS2.
  - Có chế độ chéo (ModeZ) và tinh chỉnh lệch joystick (biến i).

  PS2 (Mega 2560): DAT 28, CMD 26, SEL 24, CLK 22
*/

// Thư viện.
  #include "PS2X_lib.h"
  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <TungLam_Control_MotorV5.h>

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
  int i = 0; // Bù lệch joystick.

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

// CTHT STEP (chưa dùng trong file này).
  #define LIMIT_SWITCH1_STEP1 18
  #define LIMIT_SWITCH2_STEP1 19

// Biến STEP (chưa dùng trong file này).
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

      // Khởi tạo motor.
      TUNGLAM_Control.Mode1();

      // Chân phụ PL5 (ví dụ cơ cấu thủ môn).
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
      Joystick();
    }

// Xử lý nút bấm.
  void Butom()
    {
      if (ps2x.ButtonPressed(PSB_START))  { Serial.println("Nhấn nút START"); }
      if (ps2x.ButtonPressed(PSB_SELECT)) { Serial.println("Nhấn nút SELECT"); }

      // PAD_UP: chạy tiến.
      if (ps2x.Button(PSB_PAD_UP)) {
        Serial.println("Nhấn nút PAD_UP");
        TUNGLAM_Control.moveForward(speed);
      }

      // PAD_DOWN: giảm tốc lô (dự phòng).
      if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {
        Serial.println("Nhấn nút PAD_DOWN");
        if (speed2 > 10) speed2 -= 50;
        if (speed2 <= 30) speed2 = 0;
      }

      if (ps2x.ButtonPressed(PSB_PAD_LEFT))  { Serial.println("Nhấn nút PAD_LEFT"); }
      if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) { Serial.println("Nhấn nút PAD_RIGHT"); }
      if (ps2x.ButtonPressed(PSB_SQUARE))    { Serial.println("Nhấn nút SQUARE"); }
      if (ps2x.ButtonPressed(PSB_CIRCLE))    { Serial.println("Nhấn nút CIRCLE"); }
      if (ps2x.ButtonPressed(PSB_TRIANGLE))  { Serial.println("Nhấn nút TRIANGLE"); }
      if (ps2x.ButtonPressed(PSB_CROSS))     { Serial.println("Nhấn nút CROSS"); }

      // L1: đổi tốc độ di chuyển.
      if (ps2x.ButtonPressed(PSB_L1)) {
        Serial.println("Nhấn nút L1");
        mode = !mode;
        if (mode == 1) { speed = 255; speed6 = 200; }
        if (mode == 0) { speed = 200; speed6 = 180; }
      }

      // R1: toggle cơ cấu phụ (PL5).
      if (ps2x.ButtonPressed(PSB_R1)) {
        PORTL ^= (1 << PL5);
      }

      // R2: dự phòng.
      if (ps2x.ButtonPressed(PSB_R2)) {
        Serial.println("Nhấn nút R2");
      }

      // L2: bật/tắt chế độ chéo.
      if (ps2x.ButtonPressed(PSB_L2)) {
        Serial.println("Nhấn nút L2");
        ModeZ = !ModeZ;
      }
    }

// Xử lý joystick.
  void Joystick()
    {
      // Bù lệch bằng biến i.
      LX2 = ps2x.Analog(PSS_LX) - 128 + i;
      LY2 = ps2x.Analog(PSS_LY) - 127 - i;
      RX2 = ps2x.Analog(PSS_RX) - 128 + i;
      RY2 = ps2x.Analog(PSS_RY) - 127 - i;

      // Debug.
      Serial.print(LX2);
      Serial.print("  ");
      Serial.print(LY2);
      Serial.print("  ");
      Serial.print(RX2);
      Serial.print("  ");
      Serial.println(RY2);

      // Vùng chết để tránh nhiễu.
      if (abs(LX2) < 20 && abs(LY2) < 20 && abs(RX2) < 20 && abs(RY2) < 20) {
        TUNGLAM_Control.ABS(speed);
        Serial.println("stop");
      }
      if (abs(LX2) < 20 && LY2 < -100) { TUNGLAM_Control.moveForward(speed);  Serial.println("t"); }
      if (abs(LX2) < 20 && LY2 > 100)  { TUNGLAM_Control.moveBackward(speed); Serial.println("l"); }
      if (LX2 > 100 && abs(LY2) < 20)  { TUNGLAM_Control.moveRightSide(speed);Serial.println("Np"); }
      if (LX2 < -100 && abs(LY2) < 20) { TUNGLAM_Control.moveLeftSide(speed); Serial.println("Nt"); }
      if (RX2 > 100 && abs(RY2) < 20)  { TUNGLAM_Control.moveRight(speed6);   Serial.println("p"); }
      if (RX2 < -100 && abs(RY2) < 20) { TUNGLAM_Control.moveLeft(speed6);    Serial.println("tr"); }
      if (abs(RX2) < 20 && RY2 < -100) { TUNGLAM_Control.moveForward(speed);  Serial.println("t"); }
      if (abs(RX2) < 20 && RY2 > 100)  { TUNGLAM_Control.moveBackward(speed); Serial.println("l"); }

      // Chế độ chéo.
      if (ModeZ == 1)
        {
          if (LX2 == -128 + i && LY2 == -127 - i) { TUNGLAM_Control.Forward_Left(speed); }
          if (LX2 == 127 + i && LY2 == -127 - i)  { TUNGLAM_Control.Forward_Right(speed); }
          if (LX2 == -128 + i && LY2 == 128 - i)  { TUNGLAM_Control.Backward_Left(speed); }
          if (LX2 == 127 + i && LY2 == 128 - i)   { TUNGLAM_Control.Backward_Right(speed); }
        }
      delay(10);
    }
