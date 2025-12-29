/*
  Xử lý joystick PS2 cho robot omni.
*/

void Joystick()
    {
    // Đọc giá trị analog.
    LX2 = ps2x.Analog(PSS_LX) - 128;
    LY2 = ps2x.Analog(PSS_LY) - 127;
    RX2 = ps2x.Analog(PSS_RX) - 128;
    RY2 = ps2x.Analog(PSS_RY) - 127;

    // Nếu thả tay và không xoay auto -> ABS.
    if (LX2 == 0 && LY2 == 0 && RX2 == 0 && RY2 == 0 && !xoay) { TUNGLAM_Control.ABS(speed); }

    // Quay tại chỗ bằng cần phải.
    if (RX2 == 127 && RY2 == 0)  { TUNGLAM_Control.moveRight(speed6); }
    if (RX2 == -128 && RY2 == 0) { TUNGLAM_Control.moveLeft(speed6); }

    // Chế độ điều khiển thường (ModeZ = 0).
    if (ModeZ == 0)
      {
        if (LX2 == 0 && LY2 == -127) { TUNGLAM_Control.moveBackward(speed); }
        if (LX2 == 0 && LY2 == 128)  { TUNGLAM_Control.moveForward(speed); }
        if (LX2 == 127 && LY2 == 0)  { TUNGLAM_Control.moveLeftSide(speed); }
        if (LX2 == -128 && LY2 == 0) { TUNGLAM_Control.moveRightSide(speed); }
        if (RX2 == 0 && RY2 == -127) { TUNGLAM_Control.moveBackward(speed); }
        if (RX2 == 0 && RY2 == 128)  { TUNGLAM_Control.moveForward(speed); }
      }

    // Chế độ đảo hướng (ModeZ = 1).
    if (ModeZ == 1)
      {
        if (LX2 == 0 && LY2 == -127) { TUNGLAM_Control.moveForward(speed); }
        if (LX2 == 0 && LY2 == 128)  { TUNGLAM_Control.moveBackward(speed); }
        if (LX2 == 127 && LY2 == 0)  { TUNGLAM_Control.moveRightSide(speed); }
        if (LX2 == -128 && LY2 == 0) { TUNGLAM_Control.moveLeftSide(speed); }
        if (RX2 == 0 && RY2 == -127) { TUNGLAM_Control.moveForward(speed); }
        if (RX2 == 0 && RY2 == 128)  { TUNGLAM_Control.moveBackward(speed); }

        // Các hướng chéo.
        if (LX2 == -128 && LY2 == -127) { TUNGLAM_Control.Forward_Left(speed); }
        if (LX2 == 127 && LY2 == -127)  { TUNGLAM_Control.Forward_Right(speed); }
        if (LX2 == -128 && LY2 == 128)  { TUNGLAM_Control.Backward_Left(speed); }
        if (LX2 == 127 && LY2 == 128)   { TUNGLAM_Control.Backward_Right(speed); }
        if (RX2 == -128 && RY2 == -127) { TUNGLAM_Control.Forward_Left(speed); }
        if (RX2 == 127 && RY2 == -127)  { TUNGLAM_Control.Forward_Right(speed); }
        if (RX2 == -128 && RY2 == 128)  { TUNGLAM_Control.Backward_Left(speed); }
        if (RX2 == 127 && RY2 == 128)   { TUNGLAM_Control.Backward_Right(speed); }
      }

    delay(10);
    }
