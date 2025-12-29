/*
  Mecanum_STEP_Roboball - Robot mecanum + cơ cấu STEP nâng/hạ
  Tác giả: Tùng Lâm Automatic

  Mô tả nhanh:
  - Điều khiển di chuyển bằng tay cầm PS2.
  - Có cơ cấu STEP nâng/hạ và 2 động cơ DC (hút/bắn).

  Sơ đồ chân chính (Arduino Mega 2560):
  - PS2: DAT 28, CMD 26, SEL 24, CLK 22
  - STEP1: PUL = PE4 (D2), DIR = PE5 (D3), ENA = PL7 (D42)
  - CTHT STEP1: D18 (LIMIT_SWITCH1), D19 (LIMIT_SWITCH2)
  - DC hút/bắn: PB5 (D11), PB4 (D10)
*/

// Khai báo thư viện.
  #include "PS2X_lib.h"               // Thư viện tay cầm PS2.
  #include <avr/io.h>                 // Thanh ghi AVR.
  #include <avr/interrupt.h>          // Hỗ trợ ngắt.
  #include <TungLam_Control_MotorV5.h> // Thư viện điều khiển motor.

// Khai báo đối tượng điều khiển motor.
  TungLam_Control_MotorV5 TUNGLAM_Control;

// Khai báo chân tay cầm PS2.
  #define PS2_DAT 28
  #define PS2_CMD 26
  #define PS2_SEL 24
  #define PS2_CLK 22

// Biến tay cầm PS2.
  int LX, LY, RX, RY;
  int LX2, LY2, RX2, RY2;
  PS2X ps2x;
  int error = 0;
  byte type = 0;
  byte vibrate = 0;

// Biến di chuyển động cơ bánh xe.
  uint8_t State_DC10 = 0;  // Bước thay đổi tốc độ.
  uint8_t pre;             // Lưu hướng trước đó để ABS.
  bool mode = false;       // Đổi tốc độ di chuyển.
  bool mode1 = false;      // Đổi tốc độ cho lô.
  bool isMoving = false;   // Cờ trạng thái di chuyển.
  bool L2_State = false;   // Cờ đảo trạng thái bắn.
  uint8_t speed;           // Tốc độ di chuyển.
  uint8_t speed6;          // Tốc độ xoay.
  uint8_t speed2 = 100;    // Tốc độ lô bắn.
  uint8_t speed1 = mode1 ? 0 : 255; // Tốc độ lô quay thuận.
  uint8_t Speed_Auto = 255;
  int Speed_Auto1 = 0;
  uint8_t TIM = 10;         // Thời gian ABS.
  unsigned long startTime = 0; // Mốc thời gian bắt đầu di chuyển.
  bool Set = true;

// Biến điều khiển cơ cấu phụ.
  bool DC_H  = false;  // Bật/tắt động cơ hút.
  bool DC_B  = false;  // Bật/tắt động cơ bắn.
  bool control = false; // Cờ Auto/Manual.
  bool ModeZ = 0;       // Cờ chế độ chéo.

// Khai báo công tắc hành trình STEP1 nâng/hạ.
  #define LIMIT_SWITCH1_STEP1 18 // CTHT nâng (D18).
  #define LIMIT_SWITCH2_STEP1 19 // CTHT hạ (D19).

// Biến STEP.
  int stepsPerRev = 100;   // Số bước trên 1 vòng.
  int delayTime = 500;     // Thời gian giữa các xung (us).
  int pauseTime = 500;     // Nghỉ giữa các vòng (ms).
  unsigned long key;

  volatile bool stopForward1 = false;  // Dừng khi STEP1 quay thuận.
  volatile bool stopBackward1 = false; // Dừng khi STEP1 quay nghịch.

  volatile bool stopForward2 = false;  // Dừng khi STEP2 quay thuận (chưa dùng).
  volatile bool stopBackward2 = false; // Dừng khi STEP2 quay nghịch (chưa dùng).

// Hàm setup.
  void setup()
    {
    // Serial.
      Serial.begin(115200);

    // Khởi tạo điều khiển động cơ.
      TUNGLAM_Control.Mode1();

    // Khai báo chân điều khiển DC hút/bắn (PWM D10/D11).
      DDRB |= (1 << PB5) | (1 << PB4);
      PORTB &= ~(1 << PB5); // Tắt ban đầu.
      PORTB &= ~(1 << PB4);

    // STEP1: PUL (PE4), DIR (PE5), ENA (PL7).
      DDRE |= (1 << PE4) | (1 << PE5);
      DDRL |= (1 << PL7);

    // Bật ENA STEP (tùy driver, HIGH = enable).
      PORTL |= (1 << PL7);

    // Cấu hình CTHT STEP1.
      pinMode(LIMIT_SWITCH1_STEP1, INPUT_PULLUP);
      pinMode(LIMIT_SWITCH2_STEP1, INPUT_PULLUP);

    // Ngắt CTHT STEP1.
      attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH1_STEP1), stopForwardInterrupt1, FALLING);
      attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH2_STEP1), stopBackwardInterrupt1, FALLING);

    // Cấu hình tay cầm PS2.
      Serial.print("Search Controller..");
      do {
        // GamePad(clock, command, attention, data, Pressures?, Rumble?)
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

      // Đọc vài lần cho ổn định.
      ps2x.read_gamepad(true, 200);
      delay(200);
      ps2x.read_gamepad(false, 200);
      delay(300);
      ps2x.read_gamepad(true, 200);
      delay(200);
    }

// Hàm loop.
  void loop()
    {
      ps2x.read_gamepad(false, vibrate); // Đọc tay cầm.
      Butom();                           // Xử lý nút bấm.
      Joystick();                        // Xử lý joystick.
    }
