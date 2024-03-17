#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  // обрубает дробную часть
  int flag = 0;
  if (result != NULL) {
    int minus = 0;
    s21_big_decimal value_1 = from_dec_to_big(&value);
    set_null_dec_lil(result);
    if (big_get_sign(value_1)) {
      big_set_sign(&value_1);
      minus = 1;
    }
    s21_big_decimal ten = {0};
    ten.bits[0] = 10;
    s21_big_decimal res_big = {0};
    int scale = get_scale(value);
    if (scale == 0) {
      *result = value;
    } else {
      for (int i = 0; i < scale; i++) {
        delenie_s_ostatkom(&value_1, &ten, &res_big);
        value_1 = res_big;
      }
      big_set_scale(&res_big, 0);
      if (minus) {
        big_set_sign(&res_big);
      }
      *result = from_big_to_dec(&res_big);
    }
  } else
    flag = 1;
  return flag;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int flag = 0;
  if (result != NULL) {
    set_sign(&value);
    copy(result, &value);
  } else
    flag = 1;
  return flag;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int flag = 0;
  if (result != NULL) {
    s21_decimal truncate_result = {0};
    set_null_dec_lil(result);
    s21_truncate(value, &truncate_result);  // отбрасываю дробную часть
    if (get_sign(value) &&
        get_scale(value)) {  // если число отрицательное и не целое

      s21_decimal negate_one = {0};
      s21_from_int_to_decimal(-1, &negate_one);

      s21_add(truncate_result, negate_one, result);  // то прибавляю минус 1
    } else {
      s21_truncate(value, result);
    }
  } else
    flag = 1;
  return flag;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int flag = 0;
  if (result != NULL) {
    s21_decimal truncate_result = {0};
    set_null_dec_lil(result);
    int znak = 0;
    if (get_sign(value)) {
      set_sign(&value);
      znak = 1;
    }
    s21_truncate(value, &truncate_result);  // отбрасываю дробную часть у value
    // printf("\nотбрасываю дробную часть у value \n");
    // print_decimal_bits(truncate_result);

    s21_decimal fractional_part = {0};
    // вычитаю целую части и оставляю только дробную
    s21_sub(value, truncate_result, &fractional_part);
    // printf("\nвычитаю целую части и оставляю только дробную\n");
    // print_decimal_bits(fractional_part);

    // домножаю на 10, чтобы перевести одну цифру из дробной части в целую
    s21_big_decimal ten_result2 = from_dec_to_big(&fractional_part);
    mul10(&ten_result2);

    s21_decimal ten_result = from_big_to_dec(&ten_result2);
    // printf("\nдомножаю дробную часть на 10\n");
    // print_decimal_bits(ten_result);

    s21_decimal truncate_result2 = {0};
    // отбрасываю дробную часть, оставляя одну цифру после точки
    s21_truncate(ten_result, &truncate_result2);
    // printf("\nотбрасываю дробную часть, оставляя одну цифру после точки\n");
    //  print_decimal_bits(truncate_result2);

    int number = 0;
    s21_from_decimal_to_int(truncate_result2,
                            &number);  // первая цифра после точки

    // printf("\nпервая цифра после точки %d\n",number);

    s21_decimal one = {0};
    s21_from_int_to_decimal(1, &one);

    if (number >= 5) {
      s21_add(
          truncate_result, one,
          result);  // если цифра после точки >=5, то прибавляю 1 к целой части
      if (znak) set_sign(result);
    } else {
      s21_truncate(value, result);  // иначе, оставляю как есть
      if (znak) set_sign(result);
    }
  } else
    flag = 1;
  return flag;
}

/*!
  \brief Копирование из b в a
*/
void copy(s21_decimal *a, s21_decimal *b) {
  for (int i = 0; i <= 3; i++) {
    a->bits[i] = b->bits[i];
  }
}

void set_null_dec_lil(s21_decimal *value_1) {
  for (int num = 3; num >= 0; num--) {  // от старшего инта/бита к младшему
    for (int i = 32; i > 0; i--) {
      set_bit(&(value_1->bits[num]), i, 0);
    }
  }
}

int check_max_value(s21_decimal value_1, s21_decimal value_2) {
  int flag = 0;
  s21_decimal value_3 = {
      {0b11111111111111111111111111111111, 0b11111111111111111111111111111111,
       0b11111111111111111111111111111111, 0b00000000000000000000000000000000}};
  s21_decimal value_4 = {
      {0b11111111111111111111111111111111, 0b11111111111111111111111111111111,
       0b11111111111111111111111111111111, 0b10000000000000000000000000000000}};
  if (s21_is_equal(value_1, value_3)) {
    flag = 1;
  } else if ((s21_is_equal(value_2, value_3))) {
    flag = 2;
  } else if ((s21_is_equal(value_1, value_4))) {
    flag = 3;
  } else if ((s21_is_equal(value_2, value_4))) {
    flag = 4;
  }
  return flag;
}
