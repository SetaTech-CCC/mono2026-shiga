/*
  Copyright 2026 Syuugo
  
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
 * ・getPot()
 *   半固定抵抗の値を返す。
 * 
 * ・stepper(reverse)
 *   ステッピングモーター制御関数
 *   3°ずつ動く。
 *   引数なしでは時計回りに動く。
 *   引数の reverse を true にすると、反時計回りに動く。
 * 
 * ・dc(action, fast)
 *   DCモーターを制御する。
 *   action には、RT(Right Turn：右回り)、LT(Left Turn：左回り)、S(Stop：即停止)、F(Free：減速)がある。
 *   aciton の文字にダブルクォーテーションは不要。
 *   fast を false にすると、回転速度が遅くなる。
 *
 * ・dcVol(action, level)
 *   規定では半固定抵抗で速度を同期する版。
 *   action の引数は同じ。
 *   level は、半固定抵抗以外で PWM 制御を行いたいときに使用。
 * 
 * ・buzz(level, duration)
 *   ブザー鳴動関数。
 *   level は周波数で、定義を要確認。ドレミを入れる。
 *   これらにダブルクォーテーションは不要。
 +   特定の周波数を数値で直接入れることも可能。
 *   音の長さは２つ目の引数である duration に秒数で入れる。小数第一位まで対応。
 * 
 * ・seg(left, center, right)
 *   7セグ制御関数。
 *   以下は引数の例：
 *   番号：num[8] (0〜9)
 *   アルファベット：Seg::A (ABCDEFのみ) または num[10] (10〜15で16進数)
 *   特定のセグ：(L1 + R2 + C3) (L1,L2,C1,C2,C3,R1,R2,DP)
 *   セグ右下の小数点は DP を使用する。
 *   消灯は Seg::X を入れる。
 * 
 * ・led(color)
 *   LED制御関数。
 *   color は、R,G,B,W,C,Y,M,K を入れる。
 * 
 * ・ledVol(color, level)
 *   規定では半固定抵抗で明るさを同期する版。
 *   color の引数は同じ。
 *   level は、半固定抵抗以外で PWM 制御を行いたいときに使用。
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
 *   引数は、LE, RI の２つある。
 *   引数の文字にダブルクォーテーションは不要。
 * 
 * ・isTactPressed(side)
 *   タクトスイッチが押された一瞬のみ true を返す。
 *   引数は Enabled 版と同じ。
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
const byte STEPPER_1_PIN = 30;  // CN4-2
const byte STEPPER_2_PIN = 31;  // CN4-3
const byte STEPPER_3_PIN = 32;  // CN4-4
const byte STEPPER_4_PIN = 33;  // CN4-5
// DCモーター
const byte DC_1_PIN = 10;  // CN4-6
const byte DC_2_PIN = 11;  // CN4-7
// 出力ピン
const byte PIN_WRITE[] = { LED_G_PIN, LED_B_PIN, LED_R_PIN, BUZZER_PIN, SRCLK_PIN, SER_PIN, RCLK_PIN, STEPPER_1_PIN, STEPPER_2_PIN, STEPPER_3_PIN, STEPPER_4_PIN, DC_1_PIN, DC_2_PIN };

// フォトインタラプタ
const byte PHOTO_INTERRUPTER_PIN = 40;  // CN4-9
// タクトスイッチ
const byte TACT_LEFT_PIN = 49;
const byte TACT_RIGHT_PIN = 51;
// トグルスイッチ
const byte TOGGLE_PIN = 53;
// 可変抵抗器
const byte POTENTIOMETER_PIN = A15;
// 入力ピン
const byte PIN_READ[] = { PHOTO_INTERRUPTER_PIN, TACT_LEFT_PIN, TACT_RIGHT_PIN, POTENTIOMETER_PIN };


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

/***************
 * 処理ここから *
 ***************/
 
/*************
 * 可変抵抗器 *
 *************/

// 可変抵抗器の値を取得（値は反転）
inline word getPot() {
  return map(analogRead(POTENTIOMETER_PIN), 1023, 0, 0, 1023);
}
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
void dc(const DC action = S, const boolean fast = true) {
  // 速度調整
  uint8_t power =(action == S) ? 0 : (fast ? 255 : 63);
  // ２ピンを４パターンで制御
  analogWrite(DC_1_PIN, (action == LT || action == S) ? power : 0);
  analogWrite(DC_2_PIN, (action == RT || action == S) ? power : 0);
}

// 速度同期(PWM) DC モーター
void dcVol(const DC action = S, const byte level = map(getPot(), 0, 1023, 0, 255)) {
  // ２ピンを４パターンで制御
  analogWrite(DC_1_PIN, (action == LT || action == S) ? constrain(level, 0, 255) : 0);
  analogWrite(DC_2_PIN, (action == RT || action == S) ? constrain(level, 0, 255) : 0);
}

/**********
 * ブザー *
 **********/

// 音階
const word NOTE_C4  = 262; // ド
const word NOTE_CS4 = 277;
const word NOTE_D4  = 294; // レ
const word NOTE_DS4 = 311;
const word NOTE_E4  = 330; // ミ
const word NOTE_F4  = 349; // ファ
const word NOTE_FS4 = 370;
const word NOTE_G4  = 392; // ソ（低音目安）
const word NOTE_GS4 = 415;
const word NOTE_A4  = 440; // ラ
const word NOTE_AS4 = 466;
const word NOTE_B4  = 494; // シ
const word NOTE_C5  = 523;
const word NOTE_CS5 = 554;
const word NOTE_D5  = 587;
const word NOTE_DS5 = 622;
const word NOTE_E5  = 659;
const word NOTE_F5  = 698;
const word NOTE_FS5 = 740;
const word NOTE_G5  = 784; // （中音目安）
const word NOTE_GS5 = 831;
const word NOTE_A5  = 880;
const word NOTE_AS5 = 932;
const word NOTE_B5  = 988;
const word NOTE_C6  = 1047;
const word NOTE_CS6 = 1109;
const word NOTE_D6  = 1175; // （高音目安）
const word NOTE_DS6 = 1245;
const word NOTE_E6  = 1319;

// ブザー鳴動制御
void buzz(const word level = 0, const float duration = 0.0f) {
  if (level <= 0 || duration > 0.0f) {
    // 鳴動
    tone(BUZZER_PIN, level, (unsigned long) (duration * 1000.0f));
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
void led(const Color color = K) {
  for (const auto& rgb : rgb_pins)
    digitalWrite(rgb.pin, (color & rgb.color));
}

// 明るさ同期(PWM) LED
void ledVol(const Color color = K, const byte level = map(getPot(), 0, 1023, 0, 255)) {
  for (const auto& rgb : rgb_pins)
    analogWrite(rgb.pin, (color & rgb.color) ? constrain(level, 0, 255) : 0);
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

/*****************
 * タクトスイッチ *
 *****************/

// タクトスイッチの左右を識別する列挙型
enum Tact { LE, RI };
// タクトスイッチの全ピン（列挙型変数に対応）
const byte TACT_PINS[] = { TACT_LEFT_PIN, TACT_RIGHT_PIN };

// 指定された側のタクトスイッチが押され続けている時は true
boolean isTactEnabled(const Tact side) {
  return digitalRead(TACT_PINS[side]);
}

// 指定された側のタクトスイッチが１回押された時に true
boolean isTactPressed(const Tact side) {
  // 初回呼び出しフラグ
  static boolean initialized[2] = { false, false };
  // 前回の状態
  static boolean prev[2] = { LOW, LOW };
  // 最後に状態が変化した時刻
  static unsigned long last[2] = { 0, 0 };
  // 現在の状態を取得
  boolean current = isTactEnabled(side);
  // 初回は現在の状態を基準として設定し、誤検知を防ぐ
  if (!initialized[side]) {
    // 現在の状態を初期値として記録
    prev[side] = current;
    // 初期化完了
    initialized[side] = true;
    // 初回は押下とみなさない
    return false;
  }
  // 状態が変化、かつ前回の変化から <-- 20ms --> 以上経過している場合のみ有効
  if (current != prev[side] && (millis() - last[side]) > 20) {
    // 変化時刻を更新
    last[side] = millis();
    // 状態を更新
    prev[side] = current;
    // 押下なら true、離放なら false
    return current;
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
  static boolean state = false;
  // 通過回数カウンター
  static byte count = 0;
  // 羽が1枚通過した瞬間かを判定
  boolean rotate = false;
  // 現在の遮光状態を一度だけ読み取る
  boolean current = isPhotoEnabled();
  // 通過の瞬間を検知
  if (!state && current) {
    // 状態を検知済みに更新
    state = true;
    // 1回通過したと判定
    rotate = true;
  }
  // 状態をリセット
  else if (!current) {
    // 状態を未検知にリセット
    state = false;
  }
  // 1回通過を検知した場合のみ回数判断へ
  if (!rotate) {
    return false;
  }
  // 1回通過が検知された場合
  if (rotation == 1) {
    // 目標が1回なら、そのまま true
    return true;
  } else {
    // 目標が複数回なら、カウンター処理を行う
    count++;
    if (count >= rotation) {
      count = 0;
      // 目標回数に到達した
      return true;
    }
  }
  // 目標回数に達していない場合は false
  return false;
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
  // 7セグを全消灯
  seg();
  // DC モーターを停止
  dc();
  // ブザー消音
  buzz();
  // オプション関数
  start();
}

#endif  // MONO2026_H
