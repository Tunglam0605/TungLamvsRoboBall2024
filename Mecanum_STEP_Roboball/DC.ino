/*
  Điều khiển động cơ DC phụ (hút/bắn).
*/

// Hút ON/OFF (toggle chân PB5 - D11).
void PWM2()
  {
  PORTB ^= (1 << PB5); // Đảo trạng thái.
  }

// Bắn ON: PWM trên chân D10.
void PWM5()
  {
  analogWrite(10, speed2);
  }

// Bắn OFF: tắt PWM.
void PWM5_1()
  {
  analogWrite(10, 0);
  }
