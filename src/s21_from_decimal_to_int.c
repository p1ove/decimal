#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = 0;
  if (dst != NULL) {
    *dst = 0;
    int start_flag = 0;
    long double result = 0;

    for (int num = 2; num >= 0; num--) {  // от старшего инта/бита к младшему
      for (int i = 32; i > 0; i--) {
        if (get_bit(src.bits[num], i) == 1 &&
            start_flag == 0)  // поиск начала числа, т.е. первой единицы в интах
        {
          start_flag = 1;
        }

        if (start_flag && get_bit(src.bits[num], i) == 1) {
          result += pow(2.0, i - 1 + num * 32);
        }
      }
    }

    result /= pow(10.0, get_scale(src));  // учёт степени
    *dst = (int)result;
    if (get_sign(src) == 1) {
      *dst = -*dst;
    }
  } else
    flag = 1;
  return flag;
}