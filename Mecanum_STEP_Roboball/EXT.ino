/*
  Ngắt công tắc hành trình STEP1.
  INPUT_PULLUP => mức LOW là đã chạm công tắc.
*/

// CTHT nâng: dừng quay thuận.
void stopForwardInterrupt1()
  {
    PORTB &= ~(1 << PB5); // Tắt động cơ hút nếu cần.
    PORTL |= (1 << PL7);  // Disable driver (tùy mạch).
    stopForward1 = true;  // Đánh dấu dừng thuận.
  }

// CTHT hạ: dừng quay nghịch.
void stopBackwardInterrupt1()
  {
    stopBackward1 = true; // Đánh dấu dừng nghịch.
    // PORTL |= (1 << PL7); // Có thể disable driver nếu cần.
  }
