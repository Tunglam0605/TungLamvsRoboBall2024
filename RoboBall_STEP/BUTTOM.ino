/*
  Xử lý nút bấm PS2 cho RoboBall_STEP.
*/

void Butom()
  {
  if (ps2x.ButtonPressed(PSB_START))  { Serial.println("Nhấn nút START"); }
  if (ps2x.ButtonPressed(PSB_SELECT)) { Serial.println("Nhấn nút SELECT"); }

  // PAD_UP: tăng tốc lô bắn.
  if (ps2x.ButtonPressed(PSB_PAD_UP)) {
    Serial.println("Nhấn nút PAD_UP");
    if (speed2 < 245) speed2 += 10;
    if (speed2 >= 250) speed2 = 255;
  }

  // PAD_DOWN: giảm tốc lô bắn.
  if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {
    Serial.println("Nhấn nút PAD_DOWN");
    if (speed2 > 10)  speed2 -= 10;
    if (speed2 <= 30) speed2 = 0;
  }

  // PAD_LEFT/PAD_RIGHT: chọn mức preset.
  if (ps2x.ButtonPressed(PSB_PAD_LEFT)) {
    Serial.println("Nhấn nút PAD_LEFT");
    speed2 = 125;
  }
  if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) {
    Serial.println("Nhấn nút PAD_RIGHT");
    speed2 = 175;
  }

  // SQUARE: bật/tắt lô bắn.
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

  // CIRCLE: bật/tắt hút.
  if (ps2x.ButtonPressed(PSB_CIRCLE))
    {
    Serial.println("Nhấn nút CIRCLE");
    Serial.println("Hút");
    PWM2();
    }

  // TRIANGLE: dự phòng.
  if (ps2x.ButtonPressed(PSB_TRIANGLE)) { Serial.println("Nhấn nút TRIANGLE"); }

  // CROSS: toggle ENA step + chân phụ.
  if (ps2x.ButtonPressed(PSB_CROSS))
    {
    Serial.println("Nhấn nút CROSS");
    PORTL ^= (1 << PL7);
    PORTL |= (1 << PL5);
    }

  // L1: đổi tốc độ di chuyển.
  if (ps2x.ButtonPressed(PSB_L1))
    {
    Serial.println("Nhấn nút L1");
    mode = !mode;
    if (mode == 1) { speed = 255; speed6 = 200; }
    if (mode == 0) { speed = 200; speed6 = 180; }
    }

  // R1: nâng STEP1.
  if (ps2x.ButtonPressed(PSB_R1))
    {
    Serial.println("Nhấn nút R1");
    if (digitalRead(LIMIT_SWITCH1_STEP1) == HIGH)
        {
        stopForward1 = false;
        }
    if (!stopForward1) STEP1_ON(900);
    }

  // R2: hạ STEP1.
  if (ps2x.ButtonPressed(PSB_R2))
    {
    Serial.println("Nhấn nút R2");
    if (digitalRead(LIMIT_SWITCH2_STEP1) == HIGH)
      {
      stopBackward1 = false;
      }
    if (!stopBackward1) STEP1_OFF(800);
    }

  // L2: bật/tắt chế độ chéo.
  if (ps2x.ButtonPressed(PSB_L2))
    {
    Serial.println("Nhấn nút L2");
    ModeZ = !ModeZ;
    }
  }
