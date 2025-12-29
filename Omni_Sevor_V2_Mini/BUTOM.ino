/*
  Xử lý nút bấm PS2 cho Omni_Sevor_V2_Mini.
*/

void Button()
  {
    if (ps2x.ButtonPressed(PSB_START))  { Serial.println("Nhấn nút START"); }
    if (ps2x.ButtonPressed(PSB_SELECT)) { Serial.println("Nhấn nút SELECT"); }

    // PAD_LEFT: chạy auto Attack.
    if (ps2x.ButtonPressed(PSB_PAD_LEFT))
      {
      Serial.println("Nhấn nút PAD_LEFT");
      Auto = 1;
      Buoc = 0;
      timer1 = millis();
      timerz = millis();
      }

    // PAD_RIGHT: xoay tự động 180 độ.
    if (ps2x.ButtonPressed(PSB_PAD_RIGHT))
      {
      Serial.println("Nhấn nút PAD_RIGHT");
      timerx = millis();
      xoay = 1;
      }

    // PAD_UP: tăng tốc lô bắn.
    if (ps2x.ButtonPressed(PSB_PAD_UP))
      {
      Serial.println("Nhấn nút PAD_UP");
      if (speed2 < 245) speed2 += 10;
      if (speed2 >= 250) speed2 = 255;
      }

    // PAD_DOWN: giảm tốc lô bắn.
    if (ps2x.ButtonPressed(PSB_PAD_DOWN))
      {
      Serial.println("Nhấn nút PAD_DOWN");
      if (speed2 > 10)  speed2 -= 10;
      if (speed2 <= 30) speed2 = 0;
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

    // TRIANGLE: bật/tắt van/xilanh.
    if (ps2x.ButtonPressed(PSB_TRIANGLE))
      {
      Serial.println("Nhấn nút TRIANGLE");
      PORTL ^= (1 << PL5);
      }

    // CROSS: bật/tắt ghim (PD7).
    if (ps2x.ButtonPressed(PSB_CROSS))
      {
      Serial.println("Nhấn nút CROSS");
      PORTD ^= (1 << PD7);
      }

    // L1: đổi tốc độ di chuyển.
    if (ps2x.ButtonPressed(PSB_L1))
      {
      Serial.println("Nhấn nút L1");
      mode = !mode;
      if (mode == 1) { speed = 255; speed6 = 200; }
      if (mode == 0) { speed = 200; speed6 = 180; }
      }

    // R1: nâng servo.
    if (ps2x.ButtonPressed(PSB_R1))
      {
      Serial.println("Nhấn nút R1");
      servo1.write(angle_End);
      }

    // R2: hạ servo.
    if (ps2x.ButtonPressed(PSB_R2))
      {
      Serial.println("Nhấn nút R2");
      servo1.write(angle_Start);
      }

    // L2: bật/tắt chế độ chéo.
    if (ps2x.ButtonPressed(PSB_L2))
    {
      Serial.println("Nhấn nút L2");
      ModeZ = !ModeZ;
    }
  }
