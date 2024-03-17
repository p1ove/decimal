

#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int flag = 0;
  if (dst != NULL) {
    for (int i = 0; i < 4; i++) {
      dst->bits[i] = 0;
    }
    if (get_bit(src, 32)) {  // проверка на знак
      set_sign(dst);
      dst->bits[0] = ~(--src);  // ~ побитовое отрицание для отрицательных интов
    } else {
      dst->bits[0] = src;
    }
  } else
    flag = 1;
  return flag;
}