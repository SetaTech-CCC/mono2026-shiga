# 回路について

## 10ポイントRGBバーLEDアレイ

型番：`OSX10201-LRPB2`  
色相：**Red**, **Pure green**, **Blue**

- 上部５本：`CN1-2`~`CN1-6`  
- 下部５本：`CN4-2`~`CN4-6`  
- RGB：`CN9-1`(R), `CN9-2`(B), `CN9-3`(G)

点灯させたいセグメントを単に`HIGH`にすると白色で点灯する。  
色を変えたい場合、既定ではRGB全てがONの状態なので、減算方式でRGBの各ピンを`HIGH`にする。  
赤色に点灯させたい場合、`CN9-1`:`LOW`, `CN9-2`:`HIGH`, `CN9-3`:`HIGH` で点灯可能。

## 青色ドットマトリクスLED 8×8ドット

型番：`MOA20UB018GJ`

- SER(シリアル)：`CN3-2`
- SRCLK(シフト)：`CN3-3`
- RCLK(ラッチ)：`CN3-4`

シリアルで、Row(行)、Column(列)の順でデータを送る。  
Arduino の場合、`shiftOut` が最適。  
ラッチを下げ、シリアルでデータをシフトし、ラッチを上げることで適用できる。  
基板実装上、右下からの順序になるので、点灯パターンに要注意。

<details><summary>サンプル</summary>
  
```cpp
// ラッチを下げる
digitalWrite(RCLK_PIN, LOW);
// Row(行)
shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, B01010101);
// Column(列)
shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, B01010101);
// ラッチを上げる
digitalWrite(RCLK_PIN, HIGH);
```

</details>

複雑なデザインを表示させたい場合、行を配列で書き、for文で各列に一切の`delay`無しで超高速で消灯と点灯を繰り返す必要がある。

<details><summary>サンプル</summary>
  
```cpp
const byte ARROW_UP[8] = { B00001000, B00001100, B00001110, B11111111, B11111111, B00001110, B00001100, B00001000 };

// 消灯用
void matrix_reset() {
  digitalWrite(RCLK_PIN, LOW);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, B00000000);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, B00000000)
  digitalWrite(RCLK_PIN, HIGH);
}

// 1フレーム（8列分）を描画
for (byte column = 0; column < 8; column++) {
  // 残像防止のため、一旦非表示
  matrix_reset();
  // ラッチを下げる
  digitalWrite(RCLK_PIN, LOW);
  // 下から上へ
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, ARROW_UP[column]);
  // 右から左へ
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, 1 << column);
  // ラッチを上げる
  digitalWrite(RCLK_PIN, HIGH);
}
```

</details>

これを`loop`内で実行する必要がある。

## 赤色7セグメントLED表示器 1文字アノードコモン

型番：`A-551SRD`

- 左上(F)：`CN2-4`
- 左下(E)：`CN2-5`
- 中央上(A)：`CN2-9`
- 中央真ん中(G)：`CN2-3`
- 中央下(D)：`CN2-6`
- 右上(B)：`CN2-8`
- 右下(C)：`CN2-7`
- ドット(DP)：`CN2-2`
- 表示：`CN1-8`

点灯させたいセグメントと表示ピンを`HIGH`にする事で点灯可能。

## 圧電スピーカー

型番：`PKM13EPYH4000-A0`

- ブザー：`CN1-7`

Arduino の場合、`tone`関数で制御。

## DCモーター

ドライバー型番：`AE-DRV8835-S`

- AIN1/BIN1：`CN2-2` to `CN5-2`
- AIN2/BIN2：`CN2-3` to `CN5-1`
- 動作：`CN1-9`

次の表の通りにした状態で、動作ピンを`HIGH`にすると動作する。

| IN1  | IN2  | DIR   |
| ---- | ---- | ----- |
| LOW  | LOW  | FREE  |
| LOW  | HIGH | RIGHT |
| HIGH | LOW  | LEFT  |
| HIGH | HIGH | STOP  |

## ステッピングモーター

- FET1：`CN2-4` to `CN6-6`
- FET2：`CN2-5` to `CN6-5`
- FET3：`CN2-6` to `CN6-4`
- FET4：`CN2-7` to `CN6-3`
- 動作：`CN1-9`

1相励磁なので、各ピンを`HIGH`にして動作ピンを`HIGH`にする事で動作する。

| FET1 | FET2 | FET3 | FET4 |
| ---- | ---- | ---- | ---- |
| HIGH | LOW  | LOW  | LOW  |
| LOW  | HIGH | LOW  | LOW  |
| LOW  | LOW  | HIGH | LOW  |
| LOW  | LOW  | LOW  | HIGH |

反時計回りの場合は逆に回す。

Arduino の 2相励磁用の Stepper ライブラリは使用できない。

## サーボモーター

- 出力：`CN3-5` to `CN7-3`

Arduino の Servo ライブラリを使用する事で動作する。

## 透過型フォトインタラプター

型番：`SG206`

- インタラプター：`CN3-6`

Arduino の `digitalRead` で読み取り可能。  
遮光されている時が`HIGH`。

## 半固定ボリューム 10kΩ

型番：`TSR-3386T-EY5-103TR`

- 抵抗：`CN3-7`

Arduino の `analogRead` で読み取り可能。

## タクトスイッチ(大) -> 大会基板

型番：`1273HIM-160G-G`

- 左：`CN3-8`
- 右：`CN3-9`

Arduino の `digitalRead` で読み取り可能。  
ボタンが押されている時が`LOW`。

