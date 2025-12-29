/*
  Chu trình auto Attack (bắn bóng) theo timer.
*/

void Attack() {
  switch (Buoc) {
    case 0:
      // Bật lô bắn và nâng xi lanh.
      PWM5();
      PORTL |= (1 << PL5);

      if (millis() - timerz > 750) {
        servo1.write(angle_End); // Nâng bóng.
        timerz = millis();
        Buoc = 1;
      }
      break;

    case 1:
      if (millis() - timerz > 500) {
        PORTD |= (1 << PD7); // Kéo ghim.
        timerz = millis();
        Buoc = 2;
      }
      break;

    case 2:
      if (millis() - timerz > 150) {
        servo1.write(angle_End - 8); // Tinh chỉnh góc nâng.
        timerz = millis();
        Buoc = 3;
      }
      break;

    case 3:
      if (millis() - timerz > 300) {
        PORTB &= ~(1 << PB5); // Tắt hút.
        timerz = millis();
        Buoc = 4;
      }
      break;

    case 4:
      if (millis() - timerz > 700) {
        PORTD ^= (1 << PD7); // Nhả ghim.
        timerz = millis();
        Buoc = 5;
      }
      break;

    case 5:
      if (millis() - timerz > 400) {
        servo1.write(angle_Start); // Hạ bóng.
        timerz = millis();
        Buoc = 6;
      }
      break;

    case 6:
      if (millis() - timerz > 400) {
        PORTL &= ~(1 << PL5); // Hạ xi lanh.
        PWM5_1();             // Tắt lô bắn.
        timerz = millis();
        Buoc = 7;
      }
      break;

    default:
      // Kết thúc chu trình.
      break;
  }
}
