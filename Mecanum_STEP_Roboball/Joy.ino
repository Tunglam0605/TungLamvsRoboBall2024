/*
  Xử lý joystick PS2 cho robot mecanum.
*/

void Joystick()
    {
    // Đọc giá trị analog và đưa về gần 0.
    LX2 = ps2x.Analog(PSS_LX) - 128;
    LY2 = ps2x.Analog(PSS_LY) - 127;
    RX2 = ps2x.Analog(PSS_RX) - 128;
    RY2 = ps2x.Analog(PSS_RY) - 127;

    // Nếu thả tay -> ABS (dừng nhanh).
    if (LX2 == 0 && LY2 == 0 && RX2 == 0 && RY2 == 0) { TUNGLAM_Control.ABS(speed); }

    // Điều khiển theo cần trái (dịch/tiến/lùi).
    if (LX2 == 0 && LY2 == -127) { TUNGLAM_Control.Tien(speed - 5, speed - 5, speed, speed); }
    if (LX2 == 0 && LY2 == 128)  { TUNGLAM_Control.moveBackward(speed); }
    if (LX2 == 127 && LY2 == 0)  { TUNGLAM_Control.moveRightSide(speed); }
    if (LX2 == -128 && LY2 == 0) { TUNGLAM_Control.moveLeftSide(speed); }

    // Điều khiển theo cần phải (xoay/tiến/lùi).
    if (RX2 == 127 && RY2 == 0)  { TUNGLAM_Control.moveRight(speed6); }
    if (RX2 == -128 && RY2 == 0) { TUNGLAM_Control.moveLeft(speed6); }
    if (RX2 == 0 && RY2 == -127) { TUNGLAM_Control.Tien(speed - 5, speed - 5, speed, speed); }
    if (RX2 == 0 && RY2 == 128)  { TUNGLAM_Control.moveBackward(speed); }

    // Chế độ chéo (ModeZ): đi chéo theo các góc.
    if (ModeZ == 1)
      {
        if (LX2 == -128 && LY2 == -127) { TUNGLAM_Control.Forward_Left(speed); }
        if (LX2 == 127 && LY2 == -127)  { TUNGLAM_Control.Forward_Right(speed); }
        if (LX2 == -128 && LY2 == 128)  { TUNGLAM_Control.Backward_Left(speed); }
        if (LX2 == 127 && LY2 == 128)   { TUNGLAM_Control.Backward_Right(speed); }
      }

    delay(10); // Trễ nhỏ để ổn định.
    }
