#include "s21_decimal.h"
void big_move_left_1(s21_big_decimal *num);

/*!
  \brief Копирование из b в a
*/
void big_copy(s21_big_decimal *a, s21_big_decimal *b) {
  set_null_dec(a);
  for (int i = 0; i <= 7; i++) {
    a->bits[i] = b->bits[i];
  }
}

s21_big_decimal from_dec_to_big(const s21_decimal *a) {
  s21_big_decimal b = {0};
  for (int i = 0; i < 3; i++) {
    b.bits[i] = a->bits[i];
  }
  b.bits[7] = a->bits[3];
  return b;
}

s21_decimal from_big_to_dec(const s21_big_decimal *a) {
  s21_decimal b = {0};
  for (int i = 0; i < 3; i++) {
    b.bits[i] = a->bits[i];
  }
  b.bits[3] = a->bits[7];
  return b;
}

void big_move_left(s21_big_decimal *value, int move_num) {
  unsigned int high_digits = 0;
  for (int i = 0; i <= 6; i++) {
    unsigned int temp = value->bits[i];
    value->bits[i] = value->bits[i] << move_num;
    if (i != 0) value->bits[i] = value->bits[i] | high_digits;
    high_digits = temp >> (32 - move_num);
  }
}

void big_move_left_1(s21_big_decimal *num) {
  unsigned memory = 0;
  for (int i = 0; i < 7; i++) {
    unsigned temp = num->bits[i];
    num->bits[i] <<= 1;
    num->bits[i] |= memory;
    memory = temp >> (32 - 1);
  }
}

void big_move_right(s21_big_decimal *value, int move_num) {
  unsigned int low_digits = 0;
  for (int i = 6; i >= 0; i--) {
    unsigned int temp = value->bits[i];
    int move_with_sign = move_num;  // отдельная переменная, для сдвига так как
                                    // делается свдиг на 1
    // if (get_bit(value->bits[i], 32)) {  // Зануление, чтобы единица(знак), не
    //                                     // дублировалась при сдвиге
    //   value->bits[i] = value->bits[i] >> 1;
    //   move_with_sign--;
    //   set_bit(&value->bits[i], 32, 0);
    // }
    value->bits[i] = value->bits[i] >> move_with_sign;
    if (i != 6) value->bits[i] = value->bits[i] | low_digits;
    low_digits =
        (temp << (32 - move_num));  // Записали младшие разряды и свдинули
  }
}

int big_get_scale(s21_big_decimal value) {
  if (big_get_sign(value)) big_set_sign(&value);
  int scale = value.bits[7];
  scale = scale >> 16;
  // if (scale > 28) printf("ERROR: scale more than 28 in readme.md\n");
  return scale;
}

/*
Умножение на 10 мантиссы
Степень не трогает
*/
void mul10(s21_big_decimal *a) {
  s21_big_decimal b = {0};
  s21_big_decimal c = {0};

  big_copy(&b, a);
  big_copy(&c, a);
  big_move_left(&b, 3);
  big_move_left(&c, 1);
  bitwise_addition_mantiss(b, c, a);  // алгоритм умножения на 10
}

/*
Приводит числа к одной степени
Возвращает степень, которую нужно выставить результату арифметической
операции
*/
int norm(s21_big_decimal *a, s21_big_decimal *b) {
  int scale1 = big_get_scale(*a);
  int scale2 = big_get_scale(*b);

  for (; scale1 > scale2; scale2++) {
    mul10(b);
  }
  for (; scale2 > scale1; scale1++) {
    mul10(a);
  }
  return scale1;
}

/*
Приводит числа к нулевой степени
Возвращает начальную степень
*/
int normalization(s21_big_decimal *a) {
  int scale = big_get_scale(*a);
  int memo = scale;

  if (scale) {
    for (; scale > 0; scale--) {  // умножаю на 10 пока степень не станет равна
                                  // 0
      mul10(a);
    }
    big_set_scale(a, 0);
  }
  return memo;
}

/*
Зануляет бигдец
*/
void set_null_dec(s21_big_decimal *value_1) {
  for (int num = 7; num >= 0; num--) {  // от старшего инта/бита к младшему
    for (int i = 32; i > 0; i--) {
      set_bit(&(value_1->bits[num]), i, 0);
    }
  }
}

/*!
  \brief Функция, которая меняет значение степени на необходимое;
  \param[in] value Проверяемое значение децимал
  \param[in] scale Необходимая степень десятки
*/
void big_set_scale(s21_big_decimal *value, int scale) {
  int sign = get_bit(value->bits[7], 32);
  value->bits[7] = 0;
  value->bits[7] = scale << 16;
  if (sign) set_bit(&(value->bits[7]), 32, 1);
}

/*
Умножение
*/
int big_s21_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  set_null_dec(result);
  int scale = norm(&value_1, &value_2);
  s21_big_decimal temp_result = {0};

  int start_flag = 0;
  for (int num = 6; num >= 0; num--) {  // от старшего инта/бита к младшему
    for (int i = 32; i > 0; i--) {
      if (get_bit(value_2.bits[num], i) == 1 &&
          start_flag ==
              0) {  // поиск начала числа, т.е. первой единицы в интах мантиссы
        start_flag = 1;
      }

      if (start_flag) {  // если нашли начало числа
        if (get_bit(value_2.bits[num], i) ==
            1) {  // алгоритм побитового умножения
          big_copy(&temp_result, result);
          bitwise_addition_mantiss(value_1, temp_result, result);
        }
        big_move_left(result, 1);
      }
    }
  }
  big_move_right(result, 1);
  big_set_scale(result, scale);
  return 0;
}

/*
Побитовое сложение, обработка трёх ситуаций:
1+1 - перенос бита в след разряд при помощи memo
1+0 && 0+1 - просто установка бита в 1
0+0 = просто установка бита в 0
 */
void bitwise_addition(s21_big_decimal v1, s21_big_decimal v2,
                      s21_big_decimal *result) {
  set_null_dec(result);
  int scale = norm(&v1, &v2);

  unsigned int memo = 0;
  for (int num = 0; num <= 6; num++) {  // от младшего инта/бита к старшему
    for (int i = 1; i <= 32; i++) {
      unsigned int result_bit =
          get_bit(v1.bits[num], i) + get_bit(v2.bits[num], i) + memo;
      memo = result_bit /
             2;  // перенос в следующий разряд, если встретились две единицы
      result_bit %= 2;  // выставляем единицу только если один из битов единица;
      set_bit(&(result->bits[num]), i, result_bit);
    }
  }
  big_set_scale(result, scale);
}

void bitwise_addition_mantiss(s21_big_decimal v1, s21_big_decimal v2,
                              s21_big_decimal *result) {
  unsigned int memo = 0;
  for (int num = 0; num <= 6; num++) {  // от младшего инта/бита к старшему
    for (int i = 1; i <= 32; i++) {
      unsigned int result_bit =
          get_bit(v1.bits[num], i) + get_bit(v2.bits[num], i) + memo;
      memo = result_bit /
             2;  // перенос в следующий разряд, если встретились две единицы
      result_bit %= 2;  // выставляем единицу только если один из битов единица;
      set_bit(&(result->bits[num]), i, result_bit);
    }
  }
}

/*
Побитовое вычитание, обработка трёх ситуаций:
1-1 - просто установка бита в 0
1-0 - просто установка бита в 1
0-1 - перенос бита из следующего разряда при помощи memo
*/
void bitwise_subtraction(s21_big_decimal v1, s21_big_decimal v2,
                         s21_big_decimal *result) {
  set_null_dec(result);
  int scale = norm(&v1, &v2);
  // print_decimal_number(v1);
  // print_decimal_number(v2);

  unsigned int memo = 0;
  for (int num = 0; num <= 6; num++) {  // от младшего инта/бита к старшему
    for (int i = 1; i <= 32; i++) {
      int result_bit =
          get_bit(v1.bits[num], i) - get_bit(v2.bits[num], i) - memo;
      if (result_bit < 0) {  // Если результат меньше 0, значит нужно взять
                             // единицу из следующего разряда
        result_bit += 2;
        memo = 1;
      } else {
        memo = 0;
      }
      set_bit(&(result->bits[num]), i, result_bit);
    }
  }
  big_set_scale(result, scale);
}

void bitwise_subtraction_mantiss(s21_big_decimal v1, s21_big_decimal v2,
                                 s21_big_decimal *result) {
  unsigned int memo = 0;
  for (int num = 0; num <= 6; num++) {  // от младшего инта/бита к старшему
    for (int i = 1; i <= 32; i++) {
      int result_bit =
          get_bit(v1.bits[num], i) - get_bit(v2.bits[num], i) - memo;
      if (result_bit < 0) {  // Если результат меньше 0, значит нужно взять
                             // единицу из следующего разряда
        result_bit += 2;
        memo = 1;
      } else {
        memo = 0;
      }
      set_bit(&(result->bits[num]), i, result_bit);
    }
  }
}

int delenie_s_ostatkom(s21_big_decimal *v1, s21_big_decimal *v2,
                       s21_big_decimal *result) {
  set_null_dec(result);
  int scale = big_get_scale(*v1) - big_get_scale(*v2);
  big_set_scale(v1, 0);
  big_set_scale(v2, 0);

  s21_big_decimal temp_v2;
  big_copy(&temp_v2, v2);
  s21_big_decimal temp_v1;
  big_copy(&temp_v1, v1);

  while (big_s21_is_less_or_equal(*v2, *v1)) {
    big_copy(&temp_v2, v2);

    int shift = 0;
    if (big_s21_is_less_or_equal(temp_v2, *v1)) {
      while (big_s21_is_less_or_equal(temp_v2, *v1)) {
        big_move_left(&temp_v2, 1);
        shift++;
      }
      shift--;
      big_move_right(&temp_v2, 1);
    }

    s21_big_decimal res = {
        {1, 0, 0, 0, 0, 0, 0, 0}};  // то же что и result += pow(2, shift);
    for (; shift > 0; shift--) {
      big_move_left(&res, 1);
    }
    bitwise_addition_mantiss(res, *result, result);

    bitwise_subtraction_mantiss(*v1, temp_v2, v1);
  }
  big_set_scale(v1, scale);
  // big_print_decimal_bits(*v1);
  big_set_scale(result, scale);
  return 0;
}

int s21_is_big_decimal_not_empty(s21_big_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2] + dst.bits[3] + dst.bits[4] +
         dst.bits[5] + dst.bits[6];
}

/*
Функция общего деления
*/
int delenie_drobi(s21_big_decimal v1, s21_big_decimal v2,
                  s21_big_decimal *result) {
  set_null_dec(result);
  int scale = big_get_scale(v1) - big_get_scale(v2);  // pa
  delenie_s_ostatkom(&v1, &v2, result);

  s21_big_decimal temp_result = {0};
  big_copy(&temp_result, result);

  while (s21_is_big_decimal_not_empty(v1) &&
         scale < 29) {  // Остаток = v1, только в интовом представлении. Можно
    // заменить на аналогичную децимал функцию сравнения с 0.
    mul10(&v1);
    mul10(result);
    scale++;
    // printf("\njopa\n");
    delenie_s_ostatkom(&v1, &v2, &temp_result);

    s21_big_decimal temp_result2 = {
        0}; /* По сути эти три строки это result+= temp_result */
    big_copy(&temp_result2, result);
    bitwise_addition(temp_result, temp_result2, result);
  }
  if (scale < 0) return 1;
  big_set_scale(result, scale);
  // big_print_decimal_bits(*result);
  return 0;
}

int big_get_sign(s21_big_decimal value) { return get_bit(value.bits[7], 32); }

int big_s21_is_less(s21_big_decimal a, s21_big_decimal b) {
  int result = -1;

  if (big_get_sign(a) < big_get_sign(b)) {
    result = 0;
  } else if (big_get_sign(a) > big_get_sign(b)) {
    result = 1;
  } else {
    s21_big_decimal a1, b1;
    big_copy(&a1, &a);
    big_copy(&b1, &b);
    norm(&a1, &b1);
    for (int num = 6; num >= 0 && result == -1;
         num--) {  // от старшего инта к младшему
      if (a1.bits[num] == 0 && b1.bits[num] == 0) {
        continue;
      }
      if (a1.bits[num] < b1.bits[num]) {
        result = 1;
      } else if (a1.bits[num] > b1.bits[num]) {
        result = 0;
      }
    }
  }
  if (result == -1) result = 0;
  return result;
}
void big_set_sign(s21_big_decimal *value) {
  int num_zero_or_one = 1;
  if ((value->bits[7] >> 31) & 1) num_zero_or_one = 0;

  set_bit(&value->bits[7], 32, num_zero_or_one);
}

int big_s21_is_less_or_equal(s21_big_decimal a, s21_big_decimal b) {
  int result = 1;
  s21_big_decimal a1 = {0};
  s21_big_decimal b1 = {0};
  big_copy(&a1, &a);
  big_copy(&b1, &b);
  norm(&a1, &b1);

  if (big_get_sign(a) < big_get_sign(b)) {
    result = 0;
  } else if (big_get_sign(a) > big_get_sign(b)) {
    result = 1;
  } else if (big_get_sign(a) && big_get_sign(b)) {
    result = 0;
    for (int num = 6; num >= 0; num--) {
      if (a1.bits[num] == 0 && b1.bits[num] == 0) {
        result = 1;
        continue;
      }
      if (a1.bits[num] == b1.bits[num]) {
        result = 1;
        continue;
      } else if ((a1.bits[num] > b1.bits[num])) {
        result = 1;
        break;
      } else if (a1.bits[num] < b1.bits[num]) {
        result = 0;
        break;
      }
    }
  } else {
    for (int num = 6; num >= 0 && result != 0; num--) {
      if (a1.bits[num] == 0 && b1.bits[num] == 0) {
        result = 1;
        continue;
      }
      if (a1.bits[num] == b1.bits[num]) {
        result = 1;
        continue;
      } else if ((a1.bits[num] > b1.bits[num])) {
        result = 0;
        break;
        // } else if (a1.bits[num] <= b1.bits[num]) {
      } else if (a1.bits[num] < b1.bits[num]) {
        result = 1;
        break;
      }
    }
  }
  return result;
}

int big_s21_is_equal(s21_big_decimal a, s21_big_decimal b) {
  norm(&a, &b);
  int result = 0;
  for (int num = 6; num >= 0; num--) {  // от старшего инта к младшему
    if (a.bits[num] == b.bits[num]) {
      result = 1;
    } else {
      result = 0;
      break;
    }
  }
  return result;
}
