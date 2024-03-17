#include "s21_decimal.h"
/*!
  \brief Функция, которая возвращает 0 или 1 в зависимости от текущего бита.
  Если бит '0' то вернет - 0, если бит '1' то вернет - 1;
  \param[in] bit Получаемый разряд децимала -  bits[0], bits[1], bits[2],
  bits[3] \param[in] pos_bit  Позиция бита, отсчет идет от 1 до 32
*/
unsigned int get_bit(unsigned int bit, int pos_bit) {
  return (bit >> (--pos_bit)) & 1;
}