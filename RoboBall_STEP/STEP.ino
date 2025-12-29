/*
  Điều khiển STEP1 (nâng/hạ) bằng xung PUL.
*/

// STEP1 hạ.
void STEP1_OFF(int steps)
  {
  PORTL &= ~(1 << PL7); // Enable driver.
  PORTE |= (1 << PE5);  // DIR = thuận (theo wiring).
  for (int i = 0; i < steps; i++)
    {
    PORTE |= (1 << PE4);  // Tạo xung PUL.
    delayMicroseconds(delayTime);
    PORTE &= ~(1 << PE4); // Kết thúc xung.
    delayMicroseconds(delayTime);
    }
  }

// STEP1 nâng.
void STEP1_ON(int steps)
  {
  PORTL &= ~(1 << PL7); // Enable driver.
  PORTE &= ~(1 << PE5); // DIR = nghịch (theo wiring).
  for (int i = 0; i < steps; i++)
    {
    PORTE |= (1 << PE4);  // Tạo xung PUL.
    delayMicroseconds(delayTime);
    PORTE &= ~(1 << PE4); // Kết thúc xung.
    delayMicroseconds(delayTime);
    }
  }
