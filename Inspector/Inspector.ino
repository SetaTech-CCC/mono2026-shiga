#include "mono2026.h"

void start() {
}

int stepCount(const int current, const boolean increment) {
  int next = current + (increment ? 1 : -1);
  if (next > 9) return 0;
  if (next < 0) return 9;
  return next;
}

void board(const boolean toggle, const boolean left, const boolean right) {
  static byte seg_l = 0, seg_r = 0;
  if (left) seg_l = stepCount(seg_l, toggle);
  if (right) seg_r = stepCount(seg_r, toggle);
  seg(num[seg_l], (Segment) (num[seg_r] | (toggle ? DP : 0)), num[map(getPot(), 1023, 0, 0, 9)]);
}

void motor(const boolean toggle, const boolean right) {
  if (toggle && right) {
    dc(RT);
    stepper();
  } else if (toggle) {
    dc(LT, false);
    stepper(true);
  } else {
    dc(S);
  }
  if (isPhotoPassed(4 * 3)) {
    buzz(MI, 0.3);
  }
}

void light(const boolean left, const boolean right) {
  Color light;
  if (left && right) light = W;
  else if (left) light = B;
  else if (right) light = G;
  else light = R;
  ledVol(light);
}

void loop() {
  boolean toggle = isToggleEnabled();
  boolean left = isTactEnabled(LE);
  boolean right = isTactEnabled(RI);
  board(toggle, left, right);
  motor(toggle, right);
  light(left, right);
}