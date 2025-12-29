/*
  Xử lý joystick PS2 cho RoboBall_STEP.
*/

void Joystick()
    {
    // Đọc analog.
    LX2 = ps2x.Analog(PSS_LX) - 128;
    LY2 = ps2x.Analog(PSS_LY) - 127;
    RX2 = ps2x.Analog(PSS_RX) - 128;
    RY2 = ps2x.Analog(PSS_RY) - 127;

    // Debug giá trị.
    Serial.print(LX2);
    Serial.print("  ");
    Serial.print(LY2);
    Serial.print("  ");
    Serial.print(RX2);
    Serial.print("  ");
    Serial.println(RY2);

    // Điều khiển cơ bản.
    if (LX2 == 0 && LY2 == 0 && RX2 == 0 && RY2 == 0) { TUNGLAM_Control.ABS(speed); }
    if (LX2 == 0 && LY2 == -127) { TUNGLAM_Control.moveForward(speed); }
    if (LX2 == 0 && LY2 == 128)  { TUNGLAM_Control.moveBackward(speed); }
    if (LX2 == 127 && LY2 == 0)  { TUNGLAM_Control.moveRightSide(speed); }
    if (LX2 == -128 && LY2 == 0) { TUNGLAM_Control.moveLeftSide(speed); }
    if (RX2 == 127 && RY2 == 0)  { TUNGLAM_Control.moveRight(speed6); }
    if (RX2 == -128 && RY2 == 0) { TUNGLAM_Control.moveLeft(speed6); }
    if (RX2 == 0 && RY2 == -127) { TUNGLAM_Control.moveForward(speed); }
    if (RX2 == 0 && RY2 == 128)  { TUNGLAM_Control.moveBackward(speed); }

    // Chế độ chéo.
    if (ModeZ == 1)
      {
        if (LX2 == -128 && LY2 == -127) { TUNGLAM_Control.Forward_Left(speed); }
        if (LX2 == 127 && LY2 == -127)  { TUNGLAM_Control.Forward_Right(speed); }
        if (LX2 == -128 && LY2 == 128)  { TUNGLAM_Control.Backward_Left(speed); }
        if (LX2 == 127 && LY2 == 128)   { TUNGLAM_Control.Backward_Right(speed); }
      }

    delay(10);
    }
