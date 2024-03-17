#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal a = from_dec_to_big(&value_1);
  s21_big_decimal b = from_dec_to_big(&value_2);
  int flag = 0;
  if (s21_is_zero_dec(a) && s21_is_zero_dec(b)) {
    flag = 0;
  } else if (check_wrong_dec(value_1) || check_wrong_dec(value_2)) {
    flag = 0;
  } else if (big_get_sign(a) && big_get_sign(b)) {
    flag = big_s21_is_less(b, a);
  } else
    flag = big_s21_is_less(a, b);
  return flag;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal a = from_dec_to_big(&value_1);
  s21_big_decimal b = from_dec_to_big(&value_2);
  int flag = 0;
  if (s21_is_zero_dec(a) && s21_is_zero_dec(b))
    flag = 1;
  else if (check_wrong_dec(value_1) || check_wrong_dec(value_2)) {
    flag = 0;
  } else
    flag = big_s21_is_less_or_equal(a, b);
  return flag;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return !(s21_is_less_or_equal(value_1, value_2));
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal a = from_dec_to_big(&value_1);
  s21_big_decimal b = from_dec_to_big(&value_2);
  int flag = 0;
  int scale = get_scale(value_1) - get_scale(value_2);

  if (scale > 0) {
    int temp = 0;
    big_set_scale(&b, scale);
    normalization(&b);
    big_set_scale(&a, 0);
    if (temp) {
      big_set_sign(&b);
    }
  } else if (scale < 0) {
    int temp = 0;
    scale *= -1;
    big_set_scale(&a, scale);
    normalization(&a);
    big_set_scale(&b, 0);
    if (temp) {
      big_set_sign(&b);
    }
  }
  if (s21_is_zero_dec(a) && s21_is_zero_dec(b)) {
    flag = 1;
  } else if (check_wrong_dec(value_1) || check_wrong_dec(value_2)) {
    flag = 0;
  } else if (get_sign(value_1) == 1 && get_sign(value_2) == 1) {
    flag = big_s21_is_equal(b, a);
  } else if (get_sign(value_1) == 0 && get_sign(value_2) == 1) {
    flag = 0;
  } else if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
    flag = 0;
  } else {
    flag = big_s21_is_equal(a, b);
  }

  return flag;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !(s21_is_equal(value_1, value_2));
}

int s21_is_zero_dec(s21_big_decimal value) {
  int result = 0;
  for (int num = 6; num >= 0; num--) {
    if (value.bits[num] == 0) {
      result = 1;
    } else {
      result = 0;
      break;
    }
  }
  return result;
}

int check_wrong_dec(s21_decimal value) {
  int flag = 0;
  for (int i = 1; i < 32; i++) {
    if (i < 17 || i > 24)
      if (get_bit(value.bits[3], i)) flag = 1;
  }
  return flag;
}
