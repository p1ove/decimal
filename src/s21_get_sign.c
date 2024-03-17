#include "s21_decimal.h"

/*!
  \brief Функция, которая возвращает 0 или 1 в зависимости от текущего знака.
  Если '+'  то вернет - 0, если '-' то вернет - 1;
  \param[in] value Проверяемое значение децимал
*/
int get_sign(s21_decimal value) { return get_bit(value.bits[3], 32); }