#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int return_value = 0;
  if (isnan(src) || isinf(src) || dst == NULL || fabs(src) >= powl(10.0, 28) ||
      fabs(src) < powl(10.0, -1 * 28)) {
    return_value = 1;
    if (dst != NULL)
      for (int i = 0; i < 4; i++) {
        dst->bits[i] = 0;
      }
  } else {
    char buffer[200] = {0};
    int temp_1 = 0;
    int temp_2 = 0;
    int scale = 0;
    s21_big_decimal integer = {0};
    s21_big_decimal real = {0};
    s21_big_decimal result = {0};
    s21_decimal result_lil = {0};
    int flag_minus = 0;
    if (src < 0.0) {
      flag_minus = 1;
      src *= -1;
    }

    snprintf(buffer, sizeof(buffer), "%.6e", src);
    sscanf(buffer, "%d.%de%d", &temp_1, &temp_2, &scale);
    scale = 6 - scale;
    integer.bits[0] = temp_1;
    real.bits[0] = temp_2;
    big_set_scale(&integer, 6);
    normalization(&integer);

    bitwise_addition(integer, real, &result);
    while (scale < 0) {
      mul10(&result);
      scale++;
    }

    result_lil = from_big_to_dec(&result);

    set_scale(&result_lil, scale);
    if (flag_minus) set_sign(&result_lil);
    for (int i = 0; i < 4; i++) {
      dst->bits[i] = result_lil.bits[i];
    }
  }
  return return_value;
}