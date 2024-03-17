# decimal
Implementation of s21_decimal.h library
# information
The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default value of a Decimal is 0. The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors. The Decimal type does not eliminate the need for rounding. Rather, it minimizes errors due to rounding.
When the result of the division and multiplication is passed to the Round method, the result suffers no loss of precision.
A decimal number is a floating-point value that consists of a sign, a numeric value where each digit in the value ranges from 0 to 9, and a scaling factor that indicates the position of a floating decimal point that separates the integral and fractional parts of the numeric value.
The binary representation of a Decimal value consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the 96-bit integer and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28. Therefore, the binary representation of a Decimal value the form, ((-2^96 to 2^96) / 10^(0 to 28)), where -(2^96-1) is equal to MinValue, and 2^96-1 is equal to MaxValue.
The scaling factor also can preserve any trailing zeros in a Decimal number. Trailing zeros do not affect the value of a Decimal number in arithmetic or comparison operations.


