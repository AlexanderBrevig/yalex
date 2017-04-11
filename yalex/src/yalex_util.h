#ifndef YALEX_YALEX_UTIL_H
#define YALEX_YALEX_UTIL_H

#include <math.h>

#ifdef YALEX_DEBUG
    #define YALEXPRT() yalexPrint("YALEX> ");
    #define YDBG(x) yalexPrint( (x) );
    #define YDBGLN(x) yalexPrintln( (x) );
#else
    #define YALEXPRT() yalexPrint("YALEX> ");
    #define YDBG(x)
    #define YDBGLN(x)
#endif

static void yalexPrint(const char *format)
{
    printf(format);
}

static inline void yalexPrintln(const char *msg)
{
    yalexPrint(msg);
    yalexPrint("\r\n");
}

static void yalftoa(float num, char *fstr)
{
    *(fstr++) = (num < 0 ? '-' : ' ');
    num = num < 0 ? num - (2 * num) : num;
    int m = log10(num);
    int digit;
    float tolerance = .0001f;

    if (num > -tolerance && num < tolerance) {
        *(fstr++) = '0';
    } else {
        while (num > 0 + tolerance) {
            float weight = pow(10.0f, m);
            digit = floor(num / weight);
            num -= (digit * weight);
            *(fstr++) = '0' + digit;
            if (m == 0) {
                *(fstr++) = '.';
            }
            m--;
        }
    }
    *(fstr) = '\0';
}
#endif
