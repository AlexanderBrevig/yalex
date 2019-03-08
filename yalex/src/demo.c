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
    yalex_repl(&world, ":nset (R0S)");
    yalex_repl(&world, ":n (R0R)");
    yalex_repl(&world, ":t1set (R1S)");
    yalex_repl(&world, ":t1 (R1R)");
    yalex_repl(&world, ":t2set (R2S)");
    yalex_repl(&world, ":t2 (R2R)");
    yalex_repl(&world, ":resset (R3S)");
    yalex_repl(&world, ":res (R3R)");
    yalex_repl(&world, ":iset (R4S)");
    yalex_repl(&world, ":i (R4R)");
    yalex_repl(&world, ":fibstep (t1 t2 + resset t2 t1set res t2set i 1 + iset rec)");
    yalex_repl(&world, ":rec (i n 1 + < 'fibstep res select)");
    yalex_repl(&world, ":fib (nset 0 t1set 1 t2set 0 resset 1 iset rec)");

    while (1) {
        word[0] = 0;
        fgets(word, sizeof(word), stdin);
        yalex_repl(&world, word);
        int x = 0;
    }
    return 0;
}

int main()
{
	yalex();
	return 0;
}
