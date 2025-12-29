/*
  Ngắt công tắc hành trình STEP1.
*/

// CTHT nâng: dừng quay thuận.
void stopForwardInterrupt1() {
  PORTB &= ~(1 << PB5);
  PORTL |= (1 << PL7);  // Disable driver.
  stopForward1 = true;
}

// CTHT hạ: dừng quay nghịch.
void stopBackwardInterrupt1() {
  stopBackward1 = true;
  // PORTL |= (1 << PL7); // Có thể disable nếu cần.
}
