#include "s21_decimal.h"

/*!
  \brief Функция, которая меняет значение степени на необходимое;
  \param[in] value Проверяемое значение децимал
  \param[in] scale Необходимая степень десятки
*/
int set_scale(s21_decimal *value, int scale) {
  int sign = get_sign(*value);
  value->bits[3] = 0;
  value->bits[3] = scale << 16;
  if (sign) set_sign(value);
  return scale;
}