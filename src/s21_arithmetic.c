#include "s21_decimal.h"

int is_1_dec(s21_decimal a);
int denormalisation_overflow(s21_big_decimal *value);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal a = from_dec_to_big(&value_1);
  s21_big_decimal b = from_dec_to_big(&value_2);
  s21_big_decimal c = from_dec_to_big(result);
  set_null_dec(&c);
  int flag = 0;
  if (get_sign(value_1) == 1 && get_sign(value_2) == 1) {
    big_set_sign(&a);
    big_set_sign(&b);
    bitwise_addition(a, b, &c);
    big_set_sign(&c);
  } else if (get_sign(value_1) == 0 && get_sign(value_2) == 1) {
    big_set_sign(&b);
    if (big_s21_is_less(a, b)) {
      bitwise_subtraction(b, a, &c);
      big_set_sign(&c);
    } else {
      bitwise_subtraction(a, b, &c);
    }
  } else if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
    big_set_sign(&a);

    if (big_s21_is_less_or_equal(a, b) == 1) {
      bitwise_subtraction(b, a, &c);
    } else {
      bitwise_subtraction(a, b, &c);
      big_set_sign(&c);
    }
  } else
    bitwise_addition(a, b, &c);
  flag = denormalisation_overflow(&c);
  *result = from_big_to_dec(&c);
  return flag;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal big_value_1 = from_dec_to_big(&value_1);
  s21_big_decimal big_value_2 = from_dec_to_big(&value_2);
  s21_big_decimal big_result = from_dec_to_big(result);
  set_null_dec(&big_result);

  // Флаг направления вычитания: -1 если value_1 > value_2, 1 если value_1 <
  // value_2, иначе 0
  int flag_sign = 0;
  int flag = 0;

  // Если value_1 и value_2 имеют разные знаки
  if (get_sign(value_1) != get_sign(value_2)) {
    // Если value_1 отрицательное, а value_2 положительное
    if (get_sign(value_1)) {
      big_set_sign(&big_value_1);
      bitwise_addition(big_value_1, big_value_2, &big_result);
      flag_sign = 1;
    } else {
      big_set_sign(&big_value_2);
      bitwise_addition(big_value_1, big_value_2, &big_result);
      flag_sign = -1;
    }
  } else {
    // Если value_1 < value_2, меняем их местами и устанавливаем флаг
    // направления вычитания
    if (get_sign(value_1)) {
      big_set_sign(&big_value_1);
      big_set_sign(&big_value_2);
    }
    if (big_s21_is_less(big_value_1, big_value_2) && get_sign(value_1)) {
      bitwise_subtraction(big_value_2, big_value_1, &big_result);
    } else if (!big_s21_is_less(big_value_1, big_value_2) &&
               get_sign(value_1)) {
      bitwise_subtraction(big_value_1, big_value_2, &big_result);
      big_set_sign(&big_result);
    } else if (big_s21_is_less(big_value_1, big_value_2)) {
      bitwise_subtraction(big_value_2, big_value_1, &big_result);
      { big_set_sign(&big_result); }
    } else
      bitwise_subtraction(big_value_1, big_value_2, &big_result);
  }

  flag = denormalisation_overflow(&big_result);
  *result = from_big_to_dec(&big_result);
  if (flag_sign && flag_sign != -1) {
    set_sign(result);
  }
  // if (flag) set_null_dec(*result);

  return flag;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal a = from_dec_to_big(&value_1);
  s21_big_decimal b = from_dec_to_big(&value_2);
  s21_big_decimal c = from_dec_to_big(result);
  set_null_dec(&c);
  int flag = 0;
  if (check_max_value(value_1, value_2) == 1 ||
      check_max_value(value_1, value_2) == 3) {
    if (get_sign(value_1) == 0 && get_sign(value_2) == 1) {
      set_sign(&value_1);
      *result = value_1;
    } else if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
      *result = value_1;
    } else
      *result = value_1;
  } else if (check_max_value(value_1, value_2) == 2 ||
             check_max_value(value_1, value_2) == 4) {
    if (get_sign(value_1) == 0 && get_sign(value_2) == 1) {
      *result = value_2;
    } else if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
      set_sign(&value_2);
      *result = value_2;
    } else
      *result = value_2;
  } else {
    if (get_sign(value_1) == 1 && get_sign(value_2) == 1) {
      big_set_sign(&a);
      big_set_sign(&b);
      big_s21_mul(a, b, &c);
      *result = from_big_to_dec(&c);
    } else if (get_sign(value_1) == 0 && get_sign(value_2) == 1) {
      big_set_sign(&b);
      big_s21_mul(a, b, &c);
      big_set_sign(&c);
      *result = from_big_to_dec(&c);
    } else if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
      big_set_sign(&a);
      big_s21_mul(a, b, &c);
      big_set_sign(&c);
      *result = from_big_to_dec(&c);
    } else {
      big_s21_mul(a, b, &c);
      *result = from_big_to_dec(&c);
    }
    flag = denormalisation_overflow(&c);
  }
  // if (flag) set_null_dec(*result);

  return flag;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;

  s21_big_decimal a = from_dec_to_big(&value_1);
  s21_big_decimal b = from_dec_to_big(&value_2);
  s21_big_decimal c = from_dec_to_big(result);
  set_null_dec(&c);
  float value_float_1;
  float value_float_2;
  s21_from_decimal_to_float(value_1, &value_float_1);
  s21_from_decimal_to_float(value_2, &value_float_2);
  if (value_float_2 == 0) {
    flag = 3;
    *result = from_big_to_dec(&b);
  } else if (value_float_1 == 0 && value_float_2 != 0) {
    *result = from_big_to_dec(&a);
  } else if (is_1_dec(value_2)) {
    *result = from_big_to_dec(&b);
  } else if (get_sign(value_1) == 1 && get_sign(value_2) == 1) {
    big_set_sign(&a);
    big_set_sign(&b);
    delenie_drobi(a, b, &c);
    *result = from_big_to_dec(&c);
  } else if (get_sign(value_1) == 0 && get_sign(value_2) == 1) {
    big_set_sign(&b);
    delenie_drobi(a, b, &c);
    big_set_sign(&c);
    *result = from_big_to_dec(&c);
  } else if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
    big_set_sign(&a);
    delenie_drobi(a, b, &c);
    big_set_sign(&c);
    *result = from_big_to_dec(&c);
  } else {
    delenie_drobi(a, b, &c);
    *result = from_big_to_dec(&c);
  }

  if (flag != 3) flag = denormalisation_overflow(&c);

  return flag;
}

int is_null_dec(s21_decimal a) {
  int null_flag = 0;
  for (int num = 2; num >= 0; num--) {
    if (a.bits[num] == 0) {
      null_flag += 1;
    }
  }
  if (null_flag == 3)
    null_flag = 1;
  else
    null_flag = 0;
  return null_flag;
}

int is_1_dec(s21_decimal a) {
  int flag = 0;
  int zero_flag = 0;
  for (int num = 2; num >= 1; num--) {
    if (a.bits[num] != 0) {
      zero_flag = 1;
    }
  }
  if (a.bits[0] == 1 && zero_flag == 0) flag = 1;
  return flag;
}

int check_overflow_bits(s21_big_decimal value) {
  int flag = value.bits[3] > 0 || value.bits[4] > 0 || value.bits[5] > 0;
  if (value.bits[6] > 0) flag = -1;
  return flag;
}

int denormalisation_overflow(s21_big_decimal *value) {
  s21_big_decimal result = {0};
  int sign = 0;
  if (big_get_sign(*value)) {
    sign = 1;
    big_set_sign(value);
  }
  big_copy(&result, value);
  int scale = big_get_scale(*value);
  int flag = 0;
  s21_big_decimal temp = {0};
  while ((/* big_is_null_dec(temp) || */ check_overflow_bits(result) == 1) &&
         scale > 0 && scale < 29) {
    s21_big_decimal ten = {0};
    big_copy(&temp, &result);
    ten.bits[0] = 10;
    delenie_s_ostatkom(&temp, &ten, &result);
    scale--;
  }
  if (check_overflow_bits(result) && scale == 0) {
    flag = 1;
    if (sign) {
      flag = 2;
    }
    set_null_dec(value);
  } else if ((scale >= 29 || check_overflow_bits(result)) &&
             big_get_sign(*value)) {
    flag = 2;
    set_null_dec(value);
  } else {
    big_set_scale(&result, scale);
    if (sign) {
      big_set_sign(&result);
    }
    big_copy(value, &result);
  }
  return flag;
}
