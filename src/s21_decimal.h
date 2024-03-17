#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct  // или использовать инт64
{
  unsigned int bits[8];
} s21_big_decimal;

// Вспомогательные функции

unsigned int get_bit(unsigned int bit, int pos_bit);
void set_bit(unsigned int *bit, int pos_bit, short num_zero_or_one);
int get_sign(s21_decimal value);
void set_sign(s21_decimal *value);
int get_scale(s21_decimal value);
int set_scale(s21_decimal *value, int scale);
int is_null_dec(s21_decimal a);
// Вспомогательные функции big_decimal

void big_copy(s21_big_decimal *a, s21_big_decimal *b);
void set_null_dec(s21_big_decimal *value_1);
int normalization(s21_big_decimal *a);
int big_get_scale(s21_big_decimal value);
void big_set_scale(s21_big_decimal *value, int scale);
void set_null_dec(s21_big_decimal *value_1);
s21_big_decimal from_dec_to_big(const s21_decimal *a);
s21_decimal from_big_to_dec(const s21_big_decimal *a);
void mul10(s21_big_decimal *a);
// Арифметика big_dec

void bitwise_addition(s21_big_decimal v1, s21_big_decimal v2,
                      s21_big_decimal *result);
void bitwise_addition_mantiss(s21_big_decimal v1, s21_big_decimal v2,
                              s21_big_decimal *result);
void bitwise_subtraction(s21_big_decimal v1, s21_big_decimal v2,
                         s21_big_decimal *result);
void bitwise_subtraction_mantiss(s21_big_decimal v1, s21_big_decimal v2,
                                 s21_big_decimal *result);
int big_s21_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
int delenie_drobi(s21_big_decimal v1, s21_big_decimal v2,
                  s21_big_decimal *result);
void big_set_sign(s21_big_decimal *value);
int big_get_sign(s21_big_decimal value);

// Сравнение big_dec

int big_s21_is_less_or_equal(s21_big_decimal a, s21_big_decimal b);
int big_s21_is_equal(s21_big_decimal a, s21_big_decimal b);
int big_s21_is_less(s21_big_decimal a, s21_big_decimal b);

// Основные функции

int s21_round(s21_decimal value, s21_decimal *result);  // other func
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // arithmetic
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal value_1, s21_decimal value_2);  // comparison
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// Функции конвертации

int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);

// Функция проверки макс значения
int check_max_value(s21_decimal value_1, s21_decimal value_2);

// int s21_is_big_decimal_not_empty(s21_big_decimal dst);
int delenie_s_ostatkom(s21_big_decimal *v1, s21_big_decimal *v2,
                       s21_big_decimal *result);

void copy(s21_decimal *a, s21_decimal *b);
void set_null_dec_lil(s21_decimal *value_1);
int s21_is_zero_dec(s21_big_decimal value);
int check_wrong_dec(s21_decimal value);
#endif  // S21_DECIMAL_H
