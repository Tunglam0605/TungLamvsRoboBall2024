<div align="center">
  <h1>RoboBall_2024</h1>
  <p><strong>Bộ dự án điều khiển robot đá bóng 2024</strong></p>
  <p>Mecanum / Omni / Servo / STEP / PS2 Control</p>
  <p>Tác giả: Tùng Lâm Automatic</p>
  <p>
    <img src="https://img.shields.io/badge/Board-Arduino%20Mega%202560-0B7C3E" />
    <img src="https://img.shields.io/badge/Input-PS2%20Gamepad-1D4ED8" />
    <img src="https://img.shields.io/badge/Drive-Mecanum%20%2F%20Omni-F97316" />
    <img src="https://img.shields.io/badge/Actuator-Servo%20%2F%20Step-111827" />
  </p>
</div>

---

## Giới thiệu
`RoboBall_2024` là tập hợp nhiều bài test/phiên bản điều khiển robot bóng đá: mecanum/omni, cơ cấu bắn bóng, servo nâng, stepper nâng/hạ và các chế độ tự động theo timer. Mỗi thư mục con là một dự án độc lập, dùng tay cầm PS2 và thư viện điều khiển motor riêng của Tùng Lâm.

## Danh sách dự án
| Thư mục | Mô tả ngắn |
| --- | --- |
| `Mecanum_STEP_Roboball` | Mecanum + cơ cấu STEP nâng/hạ + DC hút/bắn. |
| `Meganum_ButomMove` | Điều khiển mecanum bằng nút (D-pad) để test nhanh. |
| `Omni_Sevor_V2_Mini` | Omni + servo + auto Attack theo timer. |
| `RoboBall_STEP` | Mecanum + STEP nâng/hạ + servo hỗ trợ. |
| `Thumon_Meganum` | Thủ môn mecanum, có hiệu chỉnh lệch joystick. |

## Phần cứng chung
- Arduino Mega 2560.
- Tay cầm PS2 + module PS2.
- Driver động cơ (tùy chassis).
- Stepper driver (với PUL/DIR/ENA).
- Servo (nâng bóng) và cơ cấu bắn/hút.

## Thư viện cần thiết
- `PS2X_lib` (PS2X Library by Bill Porter).
- `TungLam_Control_MotorV5` (thư viện nội bộ).
- `Servo` (thư viện chuẩn Arduino).

## Cách dùng nhanh
1. Chọn thư mục con phù hợp và mở file `.ino` chính trong Arduino IDE.
2. Cài đặt các thư viện cần thiết.
3. Chọn board `Arduino Mega 2560`, chọn đúng cổng COM.
4. Upload và kiểm tra kết nối tay cầm PS2.

> Ghi chú: Mỗi dự án có chú thích chi tiết và sơ đồ chân ngay trong file `.ino`.

## Lưu ý vận hành
- Luôn nối chung GND giữa Arduino và nguồn động cơ.
- Kiểm tra lại chiều động cơ và hướng STEP trước khi chạy.
- Nếu servo/step chạy ngược, đảo chiều trong code hoặc đổi dây.

## Tác giả
Tùng Lâm Automatic

