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
    /**/
    yalex_repl(&world, ":fibstep (R1R R2R + R3S R2R R1S R3R R2S R4R 1 + R4S rec)");
    yalex_repl(&world, ":rec (R0R R4R 1 + < 'fibstep R3R select)");
    yalex_repl(&world, ":fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S rec)");

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