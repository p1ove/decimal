#include "s21_decimal.h"
/*!
  \brief Функция, которая устанавливает бит в нужное значение
  \param[in] bit Получаемый разряд децимала -  bits[0], bits[1], bits[2],
  bits[3]
  \param[in] pos_bit  Позиция бита, отсчет идет от 1 до 32
  \param[in] num_zero_or_one  бит который нужно выставить 0 или 1
*/
void set_bit(unsigned int *bit, int pos_bit, short num_zero_or_one) {
  int digits_32[32] = {0};
  for (int i = 0; i < 32; i++) {
    digits_32[i] = 1 << i;
  }
  if (num_zero_or_one)
    *bit |= digits_32[pos_bit - 1];
  else
    *bit &= ~digits_32[pos_bit - 1];
}