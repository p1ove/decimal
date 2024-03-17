#include "s21_decimal.h"

/*!
  \brief Функция, которая возвращает значение степени в числовом эквиваленте;
  \param[in] value Проверяемое значение децимал
*/
int get_scale(s21_decimal value) {
  if (get_sign(value)) set_sign(&value);
  int scale = value.bits[3];
  scale = scale >> 16;
  return scale;
}