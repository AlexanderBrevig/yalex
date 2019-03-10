#include "yalex_interop.h"

int yalex_strlen(const char * str) {
    if (str == 0) return 0;
    int len = 0;
    while (str[len]) len++;
    return len;
}

int yalex_strcat(char * to, unsigned int size, const char * from) {
    if (to == 0 || from == 0 || size == 0) return 0;
    unsigned int i, j;
    for (i = 0; to[i] != '\0'; i++)
        ;
    for (j = 0; from[j] != '\0'; j++) {
        if (i + j > size) return 0;
        to[i + j] = from[j];
    }
    to[i + j] = '\0';
    return i + j;
}

int yalex_strcmp(const char * a, const char * b) {
    if (a == 0 || b == 0) return -1;
    int offset = 0;
    for (int i = 0; a[i] && a[i] != b[0]; i++) {
        offset++;
        if (a[offset] == b[0]) {
            return offset;
        }
    }
    for (int i = 0; ; i++) {
        if (a[i] == '\0' && b[i] == '\0') {
            return 0;
        }
        if (a[i] != b[i]) {
            return a[i] < b[i] ? -1 : 1;
        }
    }
}

int yalex_strcpy(char * to, unsigned int size, const char * from) {
    if (to == 0 || from == 0 || size == 0) return 0;
    unsigned int i = 0;
    while (i < size && from[i]) {
        to[i] = from[i];
        i++;
    }
    to[i] = from[i];
    if (i >= size) return 0;
    return 1;
}

void * yalex_memset(void *s, int c, unsigned int n) {
    if (s == 0) return 0;
    unsigned char* p = s;
    while (n--)
        *p++ = (unsigned char) c;
    return s;
}

void yalex_lltoa_s(long long num, char buf[21]) {
    if (buf == 0) return;
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
    while (num != 0 && i < 20) {
        char rem = num % 10;
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

/*-
 * Copyright (c) 2014 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

 //https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtoll.c
long long yalex_atoll_s(const char buf[22], int radix) {
    register const char *s = buf;
    register unsigned long long acc;
    register int c;
    register unsigned long long cutoff;
    register int neg = 0, any, cutlim;
    c = *s++;
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+') {
        c = *s++;
    }
    if ((radix == 0 || radix == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        radix = 16;
    }
    if (radix == 0)
        radix = c == '0' ? 8 : 10;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for longs is
     * [-2147483648..2147483647] and the input base is 10,
     * cutoff will be set to 214748364 and cutlim to either
     * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
     * a value > 214748364, or equal but the next digit is > 7 (or 8),
     * the number is too big, and we will return a range error.
     *
     * Set any if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    cutoff = neg ? YALEX_LLONG_MIN : YALEX_LLONG_MAX;
    cutlim = cutoff % (long long) radix;
    cutoff /= (long long) radix;
    for (acc = 0, any = 0;; c = *s++) {
        if (ISDIGIT(c))
            c -= '0';
        else if (ISALPHA(c))
            c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= radix)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= radix;
            acc += c;
        }
    }
    if (any < 0) {
        return neg ? YALEX_LLONG_MIN : YALEX_LLONG_MAX;
    } else if (neg) {
        return -1 * ((long long) acc);
    }
    return (long long) acc;
}
