/*
  Điều khiển động cơ DC phụ (hút/bắn).
*/

// Hút ON/OFF (toggle PB5).
void PWM2()
  {
  PORTB ^= (1 << PB5);
  }

// Bắn ON: PWM D10 + bật PB7 (đèn/relay).
void PWM5()
  {
  analogWrite(10, speed2);
  PORTB |= (1 << PB7);
  }

// Bắn OFF: tắt PWM và PB7.
void PWM5_1()
  {
  PORTB &= ~(1 << PB7);
  analogWrite(10, 0);
  }
