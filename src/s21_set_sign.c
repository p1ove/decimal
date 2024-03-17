#include "s21_decimal.h"

/*!
  \brief Функция, которая инвертирует знак децимала в зависимости от текущего
  знака, если текущий знак 0, то поменяет на 1 и наоборот; \param[in] value
  Проверяемое значение децимал
*/
void set_sign(s21_decimal *value) {
  int num_zero_or_one = 1;
  if ((value->bits[3] >> 31) & 1) num_zero_or_one = 0;

  set_bit(&value->bits[3], 32, num_zero_or_one);
}