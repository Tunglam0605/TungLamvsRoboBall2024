/*
  Xử lý nút bấm tay cầm PS2 (mecanum + cơ cấu STEP).
*/

void Butom()
  {
  // START/SELECT chỉ debug.
  if (ps2x.ButtonPressed(PSB_START))  { Serial.println("Nhấn nút START"); }
  if (ps2x.ButtonPressed(PSB_SELECT)) { Serial.println("Nhấn nút SELECT"); }

  // PAD_UP: tăng tốc lô bắn (speed2).
  if (ps2x.ButtonPressed(PSB_PAD_UP)) {
    Serial.println("Nhấn nút PAD_UP");
    if (speed2 < 245) speed2 += 25;
    if (speed2 >= 250) speed2 = 255;
  }

  // PAD_DOWN: giảm tốc lô bắn.
  if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {
    Serial.println("Nhấn nút PAD_DOWN");
    if (speed2 > 10) speed2 -= 25;
    if (speed2 <= 30) speed2 = 0;
  }

  // PAD_LEFT/PAD_RIGHT: để trống (dự phòng).
  if (ps2x.ButtonPressed(PSB_PAD_LEFT))  { Serial.println("Nhấn nút PAD_LEFT"); }
  if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) { Serial.println("Nhấn nút PAD_RIGHT"); }

  // SQUARE: bật/tắt động cơ bắn.
  if (ps2x.ButtonPressed(PSB_SQUARE))
    {
    Serial.println("Nhấn nút SQUARE");
    Serial.println(DC_B);
    DC_B = !DC_B;
    switch (DC_B) {
          case 0:
              PWM5();  Serial.println("Bắn");
              break;
          case 1:
              PWM5_1();Serial.println("Dừng bắn");
              break;
          }
    }

  // CIRCLE: bật/tắt động cơ hút.
  if (ps2x.ButtonPressed(PSB_CIRCLE))
    {
    Serial.println("Nhấn nút CIRCLE");
    Serial.println("Hút");
    PWM2();
    }

  // TRIANGLE: dự phòng.
  if (ps2x.ButtonPressed(PSB_TRIANGLE)) { Serial.println("Nhấn nút TRIANGLE"); }

  // CROSS: bật/tắt ENA step + đặt PL5 HIGH (cơ cấu phụ).
  if (ps2x.ButtonPressed(PSB_CROSS))
    {
    Serial.println("Nhấn nút CROSS");
    PORTL ^= (1 << PL7); // Toggle ENA step.
    PORTL |= (1 << PL5); // Kích chân phụ.
    }

  // L1: đổi tốc độ di chuyển/xoay.
  if (ps2x.ButtonPressed(PSB_L1))
    {
    Serial.println("Nhấn nút L1");
    mode = !mode;
    if (mode == 1) { speed = 255; speed6 = 200; }
    if (mode == 0) { speed = 200; speed6 = 180; }
    }

  // R1: nâng cơ cấu STEP1.
  if (ps2x.ButtonPressed(PSB_R1))
    {
    Serial.println("Nhấn nút R1");
    if (digitalRead(LIMIT_SWITCH1_STEP1) == HIGH) {
        stopForward1 = false; // Nếu CTHT chưa chạm thì cho phép chạy.
    }
    if (!stopForward1) STEP1_ON(900);
    }

  // R2: hạ cơ cấu STEP1.
  if (ps2x.ButtonPressed(PSB_R2))
    {
    Serial.println("Nhấn nút R2");
    if (digitalRead(LIMIT_SWITCH2_STEP1) == HIGH) {
      stopBackward1 = false; // Nếu CTHT chưa chạm thì cho phép chạy.
    }
    if (!stopBackward1) STEP1_OFF(920);
    }

  // L2: bật/tắt chế độ chéo (ModeZ).
  if (ps2x.ButtonPressed(PSB_L2))
    {
    Serial.println("Nhấn nút L2");
    ModeZ = !ModeZ;
    }
  }
