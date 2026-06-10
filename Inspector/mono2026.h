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

#ifndef MONO2026_H
#define MONO2026_H

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/***********
 * 制御ピン *
 ***********/

// LED
const byte LED_G_PIN = 2;  // CN1-2
const byte LED_B_PIN = 3;  // CN1-3
const byte LED_R_PIN = 4;  // CN1-4
// ブザー
const byte BUZZER_PIN = 5;  // CN1-5
// 7セグ
const byte SRCLK_PIN = 6;  // CN1-6
const byte SER_PIN = 7;    // CN1-7
const byte RCLK_PIN = 8;   // CN1-8
// ステッピングモーター
const byte STEPPER_1_PIN = 0;  // CN4-2
const byte STEPPER_2_PIN = 0;  // CN4-3
const byte STEPPER_3_PIN = 0;  // CN4-4
const byte STEPPER_4_PIN = 0;  // CN4-5
// DCモーター
const byte DC_1_PIN = 0;  // CN4-6
const byte DC_2_PIN = 0;  // CN4-7
// 出力ピン
const byte PIN_WRITE[] = { LED_G_PIN, LED_B_PIN, LED_R_PIN, BUZZER_PIN, SRCLK_PIN, SER_PIN, RCLK_PIN, STEPPER_1_PIN, STEPPER_2_PIN, STEPPER_3_PIN, STEPPER_4_PIN, DC_1_PIN, DC_2_PIN };

// フォトインタラプタ
const byte PHOTO_INTERRUPTER_PIN = 0;  // CN4-9
// タクトスイッチ
const byte TACT_LEFT_PIN = 49;
const byte TACT_RIGHT_PIN = 51;
// トグルスイッチ
const byte TOGGLE_PIN = 53;
// 可変抵抗器
const byte POTENTIOMETER_PIN = A15;
// 入力ピン
const byte PIN_READ[] = { PHOTO_INTERRUPTER_PIN, TACT_LEFT_PIN, TACT_RIGHT_PIN, POTENTIOMETER_PIN };

/***************
 * 処理ここから *
 ***************/

/**********************
 * ステッピングモーター *
 **********************/

// ピン配列
const byte STEPPER_PINS[] = { STEPPER_1_PIN, STEPPER_2_PIN, STEPPER_3_PIN, STEPPER_4_PIN };
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
  // 強制遅延
  delay(10);
}

/*************
 * DCモーター *
 *************/

// DCモーターの動作モードを定義する列挙型
enum DC { LT, RT, S, F };
// DC モーター制御
void dc(const DC action = S) {
  // ２ピンを４パターンで制御
  digitalWrite(DC_1_PIN, (action == LT || action == S));
  digitalWrite(DC_2_PIN, (action == RT || action == S));
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
    tone(BUZZER_PIN, level > 2 ? level : BUZZ_FREQ[level], (unsigned long)(duration * 1000.0f));
  } else {
    // 消音
    noTone(BUZZER_PIN);
  }
}

/*******
 * LED *
 *******/

// LED 制御
enum Color { R = 0x1, G = 0x2, B = 0x4 };
struct RgbPins { byte pin; Color color; };
const RgbPins rgb_pins[] = { { LED_R_PIN, R }, { LED_G_PIN, G }, { LED_B_PIN, B } };
// 白（ホワイト）
const Color W = (Color) (R | G | B);
// 水色（シアン）
const Color C = (Color) (G | B);
// 黄色（イエロー）
const Color Y = (Color) (R | G);
// 紫（マゼンタ）
const Color M = (Color) (R | B);
// 消灯（ブラック）
const Color K = (Color) 0;

// LED 制御関数
void led(const Color color = 0) {
  for (const auto& rgb : rgb_pins)
    digitalWrite(rgb.pin, (color & rgb.color));
}

/*********
 * 7セグ *
 *********/

// 7セグ の列挙型
enum Segment { L1 = 0x01, L2 = 0x02, C1 = 0x04, C2 = 0x08, C3 = 0x10, R1 = 0x20, R2 = 0x40, DP = 0x80 };
struct SegPins { byte pin; Segment mask; };
const SegPins seg_pins[] = { 
  { B00100000, L1 },
  { B00010000, L2 },
  { B00000001, C1 },
  { B01000000, C2 },
  { B00001000, C3 },
  { B00000010, R1 },
  { B00000100, R2 },
  { B10000000, DP }
};

// アルファベット
namespace Seg {
const Segment A = (Segment) (L1 | L2 | C1 | C2 | R1 | R2);
const Segment B = (Segment) (L1 | L2 | C2 | C3 | R2);
const Segment C = (Segment) (L2 | C2 | C3);
const Segment D = (Segment) (L2 | C2 | C3 | R1 | R2);
const Segment E = (Segment) (L1 | L2 | C1 | C2 | C3);
const Segment F = (Segment) (L1 | L2 | C1 | C2);
const Segment X = (Segment) 0;  // 消灯
}

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
  /* A */ Seg::A,
  /* b */ Seg::B,
  /* c */ Seg::C,
  /* d */ Seg::D,
  /* E */ Seg::E,
  /* F */ Seg::F
};

byte seg_encode(const Segment s) {
  byte out = 0;
  for (const auto& sp : seg_pins)
    if (s & sp.mask) out |= sp.pin;
  return out;
}

void seg(const Segment left = Seg::X, const Segment center = Seg::X, const Segment right = Seg::X) {
  digitalWrite(RCLK_PIN, LOW);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, seg_encode(left));
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, seg_encode(center));
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, seg_encode(right));
  digitalWrite(RCLK_PIN, HIGH);
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
enum TactSwitch { LE, RI };
// タクトスイッチの全ピン（列挙型変数に対応）
const byte TACT_PINS[] = { TACT_LEFT_PIN, TACT_RIGHT_PIN };

// 指定された側のタクトスイッチが押され続けている時は true
boolean isTactEnabled(const TactSwitch side) {
  return digitalRead(TACT_PINS[side]);
}

// 指定された側のタクトスイッチが１回押された時に true
boolean isTactPressed(const TactSwitch side) {
  // 初回呼び出しフラグ
  static boolean initialized[2] = { false, false };
  // 前回の状態
  static boolean prev_state[2] = { LOW, LOW };
  // 最後に状態が変化した時刻
  static unsigned long last_changed[2] = { 0, 0 };
  // 現在の状態を取得
  boolean currently_enabled = isTactEnabled(side);
  // 初回は現在の状態を基準として設定し、誤検知を防ぐ
  if (!initialized[side]) {
    // 現在の状態を初期値として記録
    prev_state[side] = currently_enabled;
    // 初期化完了
    initialized[side] = true;
    // 初回は押下とみなさない
    return false;
  }
  // 状態が変化、かつ前回の変化から <-- 20ms --> 以上経過している場合のみ有効          ↓
  if (currently_enabled != prev_state[side] && (millis() - last_changed[side]) > 20) {
    // 変化時刻を更新
    last_changed[side] = millis();
    // 状態を更新
    prev_state[side] = currently_enabled;
    // 押下なら true、離放なら false
    return currently_enabled;
  }
  // 変化なし、またはチャタリング
  return false;
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

/*************
 * 可変抵抗器 *
 *************/

// 可変抵抗器の値を 0 ~ 1023 で取得
inline word getPot() {
  return analogRead(POTENTIOMETER_PIN);
}

/***********
 * 実行準備 *
 ***********/

// 任意初期化
void start(void);

// 初期化
void setup() {
  // 出力ピンの割り当て
  for (byte i = 0; i < ARRAY_SIZE(PIN_WRITE); i++) pinMode(PIN_WRITE[i], OUTPUT);
  // 入力ピンの割り当て
  for (byte i = 0; i < ARRAY_SIZE(PIN_READ); i++) pinMode(PIN_READ[i], INPUT);
  // オプション関数
  start();
}

#endif  // MONO2026_H