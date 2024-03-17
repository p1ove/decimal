#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  long double dbl = 0.0;
  int flag = 0;
  if (is_null_dec(src) == 1) {
    int sign = get_sign(src);
    if (sign) {
      *dst = -0.0;
    } else {
      *dst = 0.0;
    }
  } else if (dst != NULL) {
    for (int num = 2; num >= 0; num--) {  // от старшего инта/бита к младшему
      for (int i = 32; i > 0; i--) {
        if (get_bit(src.bits[num], i) == 1) {
          dbl += pow(2.0, i - 1 + num * 32);
          ;
          // возвожу двойку в степень от 0 до 96, в
          // зависимости от положения бита в децимале
        }
      }
    }
    int scale = get_scale(src);
    for (; scale > 0; scale--) {  // делю на 10 в зависимости от степени
      dbl /= 10;
    }

    if (get_sign(src)) {  // учёт знака
      dbl = -dbl;
    }
    *dst = dbl;
  } else
    flag = 1;
  return flag;
}