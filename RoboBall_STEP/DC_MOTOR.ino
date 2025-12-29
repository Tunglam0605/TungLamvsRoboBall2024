/*
  Điều khiển DC phụ (hút/bắn) cho RoboBall_STEP.
*/

// Hút ON/OFF.
void PWM2()
  {
  PORTB ^= (1 << PB5);
  }

// Bắn ON.
void PWM5()
  {
  analogWrite(10, speed2);
  // PORTB |= (1 << PB7); // (nếu dùng đèn/relay).
  }

// Bắn OFF.
void PWM5_1()
  {
  // PORTB &= ~(1 << PB7);
  analogWrite(10, 0);
  }
