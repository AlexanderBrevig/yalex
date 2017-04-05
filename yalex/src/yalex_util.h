#ifndef YALEX_YALEX_UTIL_H
#define YALEX_YALEX_UTIL_H

#include<math.h>

extern void yalexPrint(const char * format);

static inline void yalftoa(float num, char *fstr){
	int m = log10(num);
	int digit;
	float tolerance = .0001f;

	while (num > 0 + tolerance)
	{
		float weight = pow(10.0f, m);
		digit = floor(num / weight);
		num -= (digit*weight);
		*(fstr++) = '0' + digit;
		if (m == 0)
			*(fstr++) = '.';
		m--;
	}
	*(fstr) = '\0';
}

#endif
