#include <stdio.h>
#include <string.h>

#include "yalex.h"

void replMessageCallback(const char* ptr) { 
    if (ptr && strlen(ptr)>0) { printf("%s\n", ptr); } 
}

int yalex(void) {
    yalex_world world;
    yalex_init(&world, replMessageCallback);

    replMessageCallback("welcome");
    char word[256];

    while (1) {
        word[0] = 0;
        fgets(word, sizeof(word), stdin);
        yalex_repl(&world, word);
    }
    return 0;
}

int main()
{
	yalex();
	return 0;
}