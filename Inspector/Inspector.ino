#include "mono2025.h"

void start() {
  // サーボモーターを初期位置に移動
  servo(SERVO_MIN);
}

void loop() {
  // LEDバーの位置 (列挙型変数要参照)
  static byte bar_gage = 1;
  // LEDバーの色 (列挙型変数要参照)
  static byte bar_color = 1;
  // サーボモーターの角度
  static byte pos = SERVO_MIN;
  // サーボの１ステップ
  const byte SERVO_STEP = 8;
  // トグルが下向きの時
  if (!isToggleEnabled()) {
    // DCモーターをストップ
    dc(S);
    // 7セグと半固定抵抗を同期
    syncPot();
  // トグルが上向きの時
  } else {
    // DCモーターを右回転
    dc(RT);
    // ステッピングモーターを右回転
    stepper();
  }
  // 自作基盤上の右から２番目のタクトが押された時
  if (isTactPressed(RL) && bar_gage > 1) {
    // LEDバーの位置を下げる
    bar_gage--;
    // ブザー中音鳴動
    buzz(MI, 0.2);
  // 自作基盤上の右端のタクトが押された時
  } else if (isTactPressed(RR) && bar_gage < 10) {
    // LEDバーの位置を上げる
    bar_gage++;
    // ブザー高音鳴動
    buzz(HI, 0.2);
  }
  // 自作基盤上の左端のタクトが押された時
  if (isTactPressed(LL) && bar_color > 1) {
    // LEDバーの色を一つ前に戻す
    bar_color--;
    // ブザー中音鳴動
    buzz(MI, 0.2);
  // 自作基盤上の左から２番目のタクトが押された時
  } else if (isTactPressed(LR) && bar_color < 8) {
    // LEDバーの色を一つ進める
    bar_color++;
    // ブザー高音鳴動
    buzz(HI, 0.2);
  }
  // LEDバー点灯
  bar(lineIndex[bar_gage - 1], rgbIndex[bar_color - 1]);
  // LEDマトリックスとジョイスティックを同期
  syncArrow();
  // サーボ角度増加（大会基盤上の左側のタクトが押された時）
  if (isTactPressed(TL) && pos < SERVO_MAX) pos += SERVO_STEP;
  // サーボ角度減少（大会基盤上の右側のタクトが押された時）
  if (isTactPressed(TR) && pos > SERVO_MIN) pos -= SERVO_STEP;
  // サーボ適用
  servo(pos);
  // フォトインタラプタ遮蔽時にブザー低音鳴動
  if (isPhotoEnabled()) buzz(LO, 0.3);
}
