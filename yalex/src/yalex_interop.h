#ifndef YALEX_INTEROP_H
#define YALEX_INTEROP_H

#define YALEX_LLONG_MAX (0x7FFFFFFFFFFFFFFF)
#define YALEX_LLONG_MIN (0x8000000000000000)

#define ISDIGIT(x) (x>='0' && x<='9')
#define ISUPPER(x) (x>='A' && x<='Z')
#define ISALPHA(x) (ISUPPER(x) || x>='a' && x<='z')

int yalex_strlen(const char * str);
int yalex_strcat(char * to, unsigned int size, const char * from);
int yalex_strcmp(const char * a, const char * b);
int yalex_strcpy(char * to, unsigned int size, const char * from);
void * yalex_memset(void *s, int c, unsigned int n);
long long yalex_atoll_s(const char buf[22], int radix);
void yalex_lltoa_s(long long num, char buf[22]);

#endif