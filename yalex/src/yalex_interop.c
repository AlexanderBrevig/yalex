#include "yalex_interop.h"

void yalex_lltoa_s(long long num, char buf[21], int sdummy, int rdummy) {
    char str[21];
    int i = 0;
    char isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        buf[i++] = '0';
        buf[i++] = '\0';
        return;
    }

    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits 
    while (num != 0 && i<20) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    // If number is negative, append '-' 
    if (isNegative)
        str[i++] = '-';

    for (int j = i; j > 0; --j) {
        buf[i - j] = str[j - 1];
    }
    buf[i] = '\0'; // Append string terminator 
}