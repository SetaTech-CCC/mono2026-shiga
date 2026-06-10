#include "mono2026.h"

void start() {
}

int stepCount(const int current, const boolean increment) {
  int next = current + (increment ? 1 : -1);
  if (next > 9) return 0;
  if (next < 0) return 9;
  return next;
}

void loop() {
  static byte left = 0, right = 0;
  boolean toggle = isToggleEnabled();
  if (isTactPressed(LE)) left = stepCount(left, toggle);
  if (isTactPressed(RI)) right = stepCount(right, toggle);
  seg(num[left], (Segment)(num[right] | (toggle ? DP : 0)), num[map(getPot(), 1023, 0, 0, 9)]);
}