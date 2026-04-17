/*
  Copyright 2025 Syuugo
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
      http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

/*
 *  関数の説明
 *
 * ・secs()
 *   経過時間を秒単位で返す。小数第一位まで対応。
 * 
 * ・delaySecs(time)
 *   秒単位で遅延させる。小数第一位まで対応。
 * 
 * ・stepper(reverse)
 *   ステッピングモーター制御関数
 *   3°ずつ動く。
 *   引数なしでは時計回りに動く。
 *   引数の reverse を true にすると、反時計回りに動く。
 * 
 * ・dc(action)
 *   DCモーターを制御する。
 *   action には、RT(Right Turn：右回り)、LT(Left Turn：左回り)、S(Stop：即停止)、F(Free：減速)がある。
 *   aciton の文字にダブルクォーテーションは不要。
 * 
 * ・buzz(level, duration)
 *   ブザー鳴動関数。
 *   level は周波数で、HI(高音)、MI(中音)、LO(低音)がある。
 *   これらにダブルクォーテーションは不要。
 +   特定の周波数を数値で直接入れることも可能。
 *   音の長さは２つ目の引数である duration に秒数で入れる。小数第一位まで対応。
 * 
 * ・servo(angle)
 * 　サーボモーター制御関数。
 * 　引数には何度の位置まで移動させるかを入れる。(8〜160)
 * 
 * ・seg(mask)
 *   7セグ制御関数。
 *   以下は引数の例：
 *   番号：num[8] (0〜9)
 *   アルファベット：sg::A (ABCDEFのみ) または num[10] (10〜15で16進数)
 *   特定のセグ：(L1 + R2 + C3) (L1,L2,C1,C2,C3,R1,R2,POINT)
 *   セグ右下の小数点は POINT を使用する。
 * 
 * ・matrix(pattern)
 * 　LEDマトリックス制御関数。
 * 　pattern に次の定数を入れる
 * 　mt::[UP, DOWN, LEFT, RIGHT], mt::[LEFT, UP]_[1-8]
 * 　または、byte[8] で自作のデザインを作る。
 * 
 * ・bar(line, color)
 * 　LEDバー制御関数。
 * 　line には、P[1-10] を入れる。
 * 　(P1 + P6) の様に足し算も可能。
 *   lineIndex[] で上から下に加算、lineBottomIndex[] で下から上に加算。
 * 　color は、R,G,B,W,C,Y,M,K を入れる。
 * 
 * ・isPhotoEnabled()
 *   フォトインタラプタが遮断されている間 true を返す。
 * 
 * ・isPhotoPassed(rotation)
 *   羽が引数に指定した回数通過した瞬間 true を返す。
 * 
 * ・isToggleEnabled()
 *   トグルスイッチが上向きの状態なら true を返す。
 * 
 * ・isTogglePulled()
 *   トグルスイッチが上向きになった一瞬のみ true を返す。
 * 
 * ・isTactEnabled(side)
 *   タクトスイッチが押され続けている間は true を返す。
 *   引数は、TL, TR, LL, LR, RL, RR の６つある。
 *   引数の文字にダブルクォーテーションは不要。
 * 
 * ・isTactPressed(side)
 *   タクトスイッチが押された一瞬のみ true を返す。
 *   引数は Enabled 版と同じ。
 * 
 * ・syncPot()
 * 　7セグと半固定抵抗の値を同期する。
 * 
 * ・syncArrow()
 * 　LEDマトリックスとジョイスティックの値を同期する。
 */

#ifndef MONO2025_H
#define MONO2025_H

#include <Servo.h> // lib/Servo

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/***********
 * 制御ピン *
 ***********/

// LEDバー
const byte LED_BAR_1_PIN = 22; // CN1-2
const byte LED_BAR_2_PIN = 23; // CN1-3
const byte LED_BAR_3_PIN = 24; // CN1-4
const byte LED_BAR_4_PIN = 25; // CN1-5
const byte LED_BAR_5_PIN = 26; // CN1-6
// ブザー
const byte BUZZER_PIN = 27; // CN1-7
// モード切替
const byte SEG_MODE_PIN = 28; // CN1-8
const byte MODE_PIN = 29; // CN1-9
// DCモーター
const byte DC_MOTOR_1_PIN = 30; // CN2-2 to CN5-2
const byte DC_MOTOR_2_PIN = 31; // CN2-3 to CN5-1
// ステッピングモーター
const byte STEPPER_MOTOR_1_PIN = 32; // CN2-4 to CN6-6
const byte STEPPER_MOTOR_2_PIN = 33; // CN2-5 to CN6-5
const byte STEPPER_MOTOR_3_PIN = 34; // CN2-6 to CN6-4
const byte STEPPER_MOTOR_4_PIN = 35; // CN2-7 to CN6-3
// 7セグ
const byte SEG_L1_PIN = STEPPER_MOTOR_1_PIN; // CN2-4
const byte SEG_L2_PIN = STEPPER_MOTOR_2_PIN; // CN2-5
const byte SEG_C1_PIN = 37; // CN2-9
const byte SEG_C2_PIN = DC_MOTOR_2_PIN; // CN2-3
const byte SEG_C3_PIN = STEPPER_MOTOR_3_PIN; // CN2-6
const byte SEG_R1_PIN = 36; // CN2-8
const byte SEG_R2_PIN = STEPPER_MOTOR_4_PIN; // CN2-7
const byte SEG_POINT_PIN = DC_MOTOR_1_PIN; // CN2-2
// LED マトリックス
const byte SER_PIN = 38; // CN3-2 (シリアル入力)
const byte SRCLK_PIN = 39; // CN3-3 (シフトクロック)
const byte RCLK_PIN = 40; // CN3-4 (ラッチクロック)
// サーボモーター
const byte SERVO_PIN = 41; // CN3-5 to CN7-3
// LEDバー
const byte LED_BAR_6_PIN = 2; // CN4-2
const byte LED_BAR_7_PIN = 3; // CN4-3
const byte LED_BAR_8_PIN = 4; // CN4-4
const byte LED_BAR_9_PIN = 5; // CN4-5
const byte LED_BAR_10_PIN = 6; // CN4-6
const byte LED_RED_PIN = 7; // CN9-1
const byte LED_GREEN_PIN = 8; // CN9-3
const byte LED_BLUE_PIN = 9; // CN9-2
// 書き込みピン
const byte PIN_WRITE[] = { LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN, LED_BAR_1_PIN, LED_BAR_2_PIN, LED_BAR_3_PIN, LED_BAR_4_PIN, LED_BAR_5_PIN, LED_BAR_6_PIN, LED_BAR_7_PIN, LED_BAR_8_PIN, LED_BAR_9_PIN, LED_BAR_10_PIN, BUZZER_PIN, MODE_PIN, SEG_MODE_PIN, SEG_L1_PIN, SEG_L2_PIN, SEG_C1_PIN, SEG_C2_PIN, SEG_C3_PIN, SEG_R1_PIN, SEG_R2_PIN, SEG_POINT_PIN, SER_PIN, SRCLK_PIN, RCLK_PIN };

// フォトインタラプタ
const byte PHOTO_INTERRUPTER_PIN = 42; // CN3-6
// 可変抵抗器
const byte POTENTIOMETER_PIN = A15; // CN3-7
// タクトスイッチ
const byte TACT_TEST_LEFT_PIN = 44; // CN3-8
const byte TACT_TEST_RIGHT_PIN = 45; // CN3-9
const byte TACT_LEFT_LEFT_PIN = 48; // A-3 or A-2
const byte TACT_LEFT_RIGHT_PIN = 49; // A-4 or A-3
const byte TACT_RIGHT_LEFT_PIN = 50; // A-5 or A-4
const byte TACT_RIGHT_RIGHT_PIN = 51; // A-6 or A-5
// トグルスイッチ
const byte TOGGLE_PIN = 52; // A-2 or A-6
// ジョイスティック
const byte JOYSTICK_X_PIN = A1; // A-7
const byte JOYSTICK_Y_PIN = A2; // A-8
// 読み込みピン
const byte PIN_READ[] = { PHOTO_INTERRUPTER_PIN, TOGGLE_PIN, TACT_TEST_LEFT_PIN, TACT_TEST_RIGHT_PIN, TACT_LEFT_LEFT_PIN, TACT_LEFT_RIGHT_PIN, TACT_RIGHT_LEFT_PIN, TACT_RIGHT_RIGHT_PIN, POTENTIOMETER_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN };

/***********
 * 補助関数 *
 ***********/

// 秒数計算
inline float secs() {
  return millis() / 1000.0f;
}

// 秒単位で遅延
inline void delaySecs(const float time) {
  delay((unsigned long) (time * 1000.0f));
}

// モーター処理切り替え
inline void mode(const boolean seg = false) {
  // 書換準備 (セグは常時確定)
  digitalWrite(MODE_PIN, LOW);
  // 適用 (モーターのみ)
  if (!seg) digitalWrite(MODE_PIN, HIGH);
  // セグを点消灯
  digitalWrite(SEG_MODE_PIN, seg);
}

/***************
 * 処理ここから *
 ***************/

/**********************
 * ステッピングモーター *
 **********************/

// ピン配列
const byte STEPPER_PINS[] = { STEPPER_MOTOR_1_PIN, STEPPER_MOTOR_2_PIN, STEPPER_MOTOR_3_PIN, STEPPER_MOTOR_4_PIN };
// 1相励磁の駆動パターンを2次元配列で定義
const byte STEPPER_PATTERNS[4][4] = { { HIGH, LOW, LOW, LOW }, { LOW, HIGH, LOW, LOW }, { LOW, LOW, HIGH, LOW }, { LOW, LOW, LOW, HIGH } };

// ステッピングモーター制御関数
void stepper(const boolean reverse = false) {
  // 現在のステップ位置を0から3のインデックスで管理
  static byte step_index = 0;
  // 定義したパターンを4つのピンに一括で書き込む
  for (byte i = 0; i < 4; i++)
    digitalWrite(STEPPER_PINS[i], STEPPER_PATTERNS[reverse ? (3 - step_index) : step_index][i]);
  // 次のステップのインデックスを計算
  step_index = (step_index + 1) % 4;
  // モーター処理切り替え
  mode();
}

/*************
 * DCモーター *
 *************/

// DCモーターの動作モードを定義する列挙型
enum DCMotor { LT, RT, S, F };

// DC モーター制御
void dc(const DCMotor action = S) {
  // ２ピンを４パターンで制御
  digitalWrite(DC_MOTOR_1_PIN, (action == LT || action == S));
  digitalWrite(DC_MOTOR_2_PIN, (action == RT || action == S));
  // モーター処理切り替え
  mode();
}

/**********
 * ブザー *
 **********/

// ブザーの音の種類を定義する列挙型
enum BuzzerTone { LO, MI, HI };
// 型と値を同期
const word BUZZ_FREQ[] = {
  /* 低音 */ 400,
  /* 中音 */ 800,
  /* 高音 */ 1200
};
// ブザー鳴動制御
void buzz(const word level = LO, const float duration = 0.0f) {
  if (duration > 0.0f) {
    // 鳴動
    tone(BUZZER_PIN, level > 2 ? level : BUZZ_FREQ[level], (unsigned long) (duration * 1000.0f));
  } else {
    // 消音
    noTone(BUZZER_PIN);
  }
}

/*****************
 * サーボモーター *
 *****************/

// Servo のライブラリで制御
static Servo srv;
// 可動域最小値
const byte SERVO_MIN = 8;
// 可動域最大値
const byte SERVO_MAX = 160;

// サーボモーター制御関数
void servo(const byte angle = SERVO_MIN) {
  // 適用
  srv.write(constrain(angle, SERVO_MIN, SERVO_MAX));
}

/*********
 * 7セグ *
 *********/

// 7セグ の列挙型
enum Segment { L1 = 0x01, L2 = 0x02, C1 = 0x04, C2 = 0x08, C3 = 0x10, R1 = 0x20, R2 = 0x40, POINT = 0x80 };
struct SegPins { byte pin; Segment mask; };
const SegPins seg_pins[] = { { SEG_L1_PIN, L1 }, { SEG_L2_PIN, L2 }, { SEG_C1_PIN, C1 }, { SEG_C2_PIN, C2 }, { SEG_C3_PIN, C3 }, { SEG_R1_PIN, R1 }, { SEG_R2_PIN, R2 }, { SEG_POINT_PIN, POINT } };

// int で直接描写できるように数字のみの配列を用意
const Segment num[] = {
  /* 0 */ (Segment) (L1 | L2 | C1 | C3 | R1 | R2),
  /* 1 */ (Segment) (R1 | R2),
  /* 2 */ (Segment) (L2 | C1 | C2 | C3 | R1),
  /* 3 */ (Segment) (C1 | C2 | C3 | R1 | R2),
  /* 4 */ (Segment) (L1 | C2 | R1 | R2),
  /* 5 */ (Segment) (L1 | C1 | C2 | C3 | R2),
  /* 6 */ (Segment) (L1 | L2 | C1 | C2 | C3 | R2),
  /* 7 */ (Segment) (L1 | C1 | R1 | R2),
  /* 8 */ (Segment) (L1 | L2 | C1 | C2 | C3 | R1 | R2),
  /* 9 */ (Segment) (L1 | C1 | C2 | C3 | R1 | R2),
  /* A */ (Segment) (L1 | L2 | C1 | C2 | R1 | R2),
  /* b */ (Segment) (L1 | L2 | C2 | C3 | R2),
  /* c */ (Segment) (L2 | C2 | C3),
  /* d */ (Segment) (L2 | C2 | C3 | R1 | R2),
  /* E */ (Segment) (L1 | L2 | C1 | C2 | C3),
  /* F */ (Segment) (L1 | L2 | C1 | C2)
};

// アルファベット
namespace sg {
  const Segment A = (Segment) (L1 | L2 | C1 | C2 | R1 | R2);
  const Segment B = (Segment) (L1 | L2 | C2 | C3 | R2);
  const Segment C = (Segment) (L2 | C2 | C3);
  const Segment D = (Segment) (L2 | C2 | C3 | R1 | R2);
  const Segment E = (Segment) (L1 | L2 | C1 | C2 | C3);
  const Segment F = (Segment) (L1 | L2 | C1 | C2);
  const Segment ALL_0 = (Segment) 0;
}

// セグメント実行
void seg(const byte mask = sg::ALL_0) {
  for (byte i = 0; i < ARRAY_SIZE(seg_pins); i++)
    digitalWrite(seg_pins[i].pin, (mask & seg_pins[i].mask));
  // モーター処理切り替え
  mode(true);
}

/*******************
 * LED マトリックス *
 *******************/

namespace mt {
  const byte ALL_0[8] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  const byte LEFT_1[8] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111111 };
  const byte LEFT_2[8] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000 };
  const byte LEFT_3[8] = { B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B00000000, B00000000 };
  const byte LEFT_4[8] = { B00000000, B00000000, B00000000, B00000000, B11111111, B00000000, B00000000, B00000000 };
  const byte LEFT_5[8] = { B00000000, B00000000, B00000000, B11111111, B00000000, B00000000, B00000000, B00000000 };
  const byte LEFT_6[8] = { B00000000, B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B00000000 };
  const byte LEFT_7[8] = { B00000000, B11111111, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  const byte LEFT_8[8] = { B11111111, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  const byte UP_1[8] = { B00000001, B00000001, B00000001, B00000001, B00000001, B00000001, B00000001, B00000001 };
  const byte UP_2[8] = { B00000010, B00000010, B00000010, B00000010, B00000010, B00000010, B00000010, B00000010 };
  const byte UP_3[8] = { B00000100, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100 };
  const byte UP_4[8] = { B00001000, B00001000, B00001000, B00001000, B00001000, B00001000, B00001000, B00001000 };
  const byte UP_5[8] = { B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000 };
  const byte UP_6[8] = { B00100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00100000 };
  const byte UP_7[8] = { B01000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000 };
  const byte UP_8[8] = { B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000 };
  const byte LEFT[8] = { B00011000, B00011000, B00011000, B00011000, B11111111, B01111110, B00111100, B00011000 };
  const byte RIGHT[8] = { B00011000, B00111100, B01111110, B11111111, B00011000, B00011000, B00011000, B00011000 };
  const byte UP[8] = { B00001000, B00001100, B00001110, B11111111, B11111111, B00001110, B00001100, B00001000 };
  const byte DOWN[8] = { B00010000, B00110000, B01110000, B11111111, B11111111, B01110000, B00110000, B00010000 };
}

// 消灯用
void matrix_reset() {
  digitalWrite(RCLK_PIN, LOW);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, B00000000); // 行
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, B00000000); // 列
  digitalWrite(RCLK_PIN, HIGH);
}

// 点灯
void matrix(const byte pattern[8] = mt::ALL_0) {
  // 1フレーム（8列分）を描画
  for (byte column = 0; column < 8; column++) {
    // 残像防止のため、一旦非表示
    matrix_reset();
    // ラッチピンを下げて、データ送信を開始
    digitalWrite(RCLK_PIN, LOW);
    // 行・Row（下から上へ）
    shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, pattern[column]);
    // 列・Column（右から左へ）
    shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 1 << column);
    // ラッチピンを上げて、シフトレジスタのデータに反映させる
    digitalWrite(RCLK_PIN, HIGH);
  }
}

/************
 * LED バー *
 ************/

// 各線の列挙型
enum Line : word { P1 = 0x001, P2 = 0x002, P3 = 0x004, P4 = 0x008, P5 = 0x010, P6 = 0x020, P7 = 0x040, P8 = 0x080, P9 = 0x100, P10 = 0x200 };
struct BarPins { byte pin; Line line; };
const BarPins bar_pins[] = { { LED_BAR_1_PIN, P1 }, { LED_BAR_2_PIN, P2 }, { LED_BAR_3_PIN, P3 }, { LED_BAR_4_PIN, P4 }, { LED_BAR_5_PIN, P5 }, { LED_BAR_6_PIN, P6 }, { LED_BAR_7_PIN, P7 }, { LED_BAR_8_PIN, P8 }, { LED_BAR_9_PIN, P9 }, { LED_BAR_10_PIN, P10 } };
// 各色の格納変数
const Line lineIndex[] = { P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 };
// 下から上
const Line lineBottomIndex[] = { P10, P9, P8, P7, P6, P5, P4, P3, P2, P1 };

// 各色の列挙型
enum Rgb { R = 0x1, G = 0x2, B = 0x4 };
struct RgbPins { byte pin; Rgb color; };
const RgbPins rgb_pins[] = { { LED_RED_PIN, R }, { LED_GREEN_PIN, G }, { LED_BLUE_PIN, B } };
// 白（ホワイト）
const Rgb W = (Rgb) (R | G | B);
// 水色（シアン）
const Rgb C = (Rgb) (G | B);
// 黄色（イエロー）
const Rgb Y = (Rgb) (R | G);
// 紫（マゼンタ）
const Rgb M = (Rgb) (R | B);
// 消灯（ブラック）
const Rgb K = (Rgb) 0;
// 各色の格納変数
const byte rgbIndex[] = { R, G, B, W, C, Y, M, K };

// LEDバー制御関数
void bar(const word line = 0, const byte color = 0) {
  for (const auto& led : bar_pins)
    digitalWrite(led.pin, (line & led.line) ? HIGH : LOW);
  for (const auto& rgb : rgb_pins)
    digitalWrite(rgb.pin, !(color & rgb.color));
}

/********************
 * フォトインタラプタ *
 ********************/

// フォトインタラプタが反応し続けている時は true
inline boolean isPhotoEnabled() {
  return digitalRead(PHOTO_INTERRUPTER_PIN) == LOW;
}

// フォトインタラプタの羽が指定回数通過した瞬間に true
boolean isPhotoPassed(const byte rotation = 1) {
  // 瞬間検知のための状態保持フラグ
  static boolean photo_state = false;
  // 通過回数カウンター
  static byte photo_passed_count = 0;
  // 羽が1枚通過した瞬間かを判定
  boolean one_rotate = false;
  // 現在の遮光状態を一度だけ読み取る
  boolean currently_enabled = isPhotoEnabled();
  // 通過の瞬間を検知
  if (!photo_state && currently_enabled) {
    // 状態を検知済みに更新
    photo_state = true;
    // 1回通過したと判定
    one_rotate = true;
  }
  // 状態をリセット
  else if (!currently_enabled) {
    // 状態を未検知にリセット
    photo_state = false;
  }
  // 1回通過を検知した場合のみ回数判断へ
  if (!one_rotate) {
    return false;
  }
  // 1回通過が検知された場合
  if (rotation == 1) {
    // 目標が1回なら、そのまま true
    return true;
  } else {
    // 目標が複数回なら、カウンター処理を行う
    photo_passed_count++;
    if (photo_passed_count >= rotation) {
      photo_passed_count = 0;
      // 目標回数に到達した
      return true;
    }
  }
  // 目標回数に達していない場合は false
  return false;
}

/*****************
 * トグルスイッチ *
 *****************/

// トグルスイッチが奥側の時は true
inline boolean isToggleEnabled() {
  return digitalRead(TOGGLE_PIN) == HIGH;
}

// トグルスイッチが上げられた時に true
boolean isTogglePulled() {
  // トグルの状態保持用
  static boolean toggle = true;
  // 現在の状態を取得
  boolean currently_enabled = isToggleEnabled();
  // トグルの状態を参照し更新
  if (currently_enabled && toggle) {
    // 上げられたので状態を更新
    toggle = false;
    return true;
  } else if (!currently_enabled) {
    // 下げられたので状態をリセット
    toggle = true;
    return false;
  } else {
    // 上げられているが無視
    return false;
  }
}

/*****************
 * タクトスイッチ *
 *****************/

// タクトスイッチの左右を識別する列挙型
enum TactSwitch { TL, TR, LL, LR, RL, RR };
// タクトスイッチの全ピン（列挙型変数に対応）
const byte TACT_PINS[] = { TACT_TEST_LEFT_PIN, TACT_TEST_RIGHT_PIN, TACT_LEFT_LEFT_PIN, TACT_LEFT_RIGHT_PIN, TACT_RIGHT_LEFT_PIN, TACT_RIGHT_RIGHT_PIN };

// 指定された側のタクトスイッチが押され続けている時は true
boolean isTactEnabled(const TactSwitch side) {
  return digitalRead(TACT_PINS[side]);
}

// 指定された側のタクトスイッチが１回押された時に true
boolean isTactPressed(const TactSwitch side) {
  // スイッチの状態保持用
  static boolean tact_pressed_state[6] = { LOW, LOW, LOW, LOW, LOW, LOW };
  boolean currently_enabled = isTactEnabled(side);
  // 指定されたスイッチの状態を参照し更新
  if (currently_enabled && tact_pressed_state[side]) {
    // 押されたので状態を更新
    tact_pressed_state[side] = false;
    return true;
  } else if (!currently_enabled) {
    // 離されたので状態をリセット
    tact_pressed_state[side] = true;
    return false;
  } else {
    // 押され続けているが無視
    return false;
  }
}

/*************
 * 可変抵抗器 *
 *************/

// 可変抵抗器の値を 0 ~ 1023 で取得
inline word getPot() {
  return analogRead(POTENTIOMETER_PIN);
}

// 可変抵抗器と7セグを同期
void syncPot() {
  // 1023 -> 9
  seg(num[map(getPot(), 0, 1023, 0, 9)]);
}

/*******************
 * ジョイスティック *
 *******************/

// X軸の値を取得
inline word getJoyX() {
  return analogRead(JOYSTICK_X_PIN);
}
// Y軸の値を取得
inline word getJoyY() {
  return analogRead(JOYSTICK_Y_PIN);
}
// 調整値
const word JOYSTICK_DEAD_ZONE = 250;
// 可動域最小値 + 調整値
const word THRESHOLD_LOW  = 100 + JOYSTICK_DEAD_ZONE;
// 可動域最大値 - 調整値
const word THRESHOLD_HIGH = 900 - JOYSTICK_DEAD_ZONE;
// LED マトリックスと向きを同期
void syncArrow() {
  // X軸
  word xValue = getJoyX();
  // Y軸
  word yValue = getJoyY();
  if (yValue < THRESHOLD_LOW) {
    matrix(mt::UP);
  } else if (yValue > THRESHOLD_HIGH) {
    matrix(mt::DOWN);
  } else if (xValue > THRESHOLD_HIGH) {
    matrix(mt::RIGHT);
  } else if (xValue < THRESHOLD_LOW) {
    matrix(mt::LEFT);
  } else {
    matrix(mt::ALL_0);
  }
}

/***********
 * 実行準備 *
 ***********/

// 任意初期化
void start(void);

// 初期化
void setup() {
  // ボードレートを指定
  Serial.begin(9600);
  // 出力ピンの割り当て
  for (byte i = 0; i < ARRAY_SIZE(PIN_WRITE); i++) pinMode(PIN_WRITE[i], OUTPUT);
  // 入力ピンの割り当て
  for (byte i = 0; i < ARRAY_SIZE(PIN_READ); i++) pinMode(PIN_READ[i], INPUT);
  // サーボの初期化
  srv.attach(SERVO_PIN);
  // DCモーターを停止
  dc(S);
  // LEDマトリックスを非表示
  matrix_reset();
  // オプション関数
  start();
}

#endif // MONO2025_H
