#include <stdio.h>
#include <string.h>

#include "yalex.h"

void replMessageCallback(const char* ptr) {
    if (ptr && strlen(ptr) > 0) { printf("%s\n", ptr); }
}

int yalex(void) {
    yalex_world world;
    yalex_init(&world, replMessageCallback);

    replMessageCallback("yalex 1.0beta");

    yalex_repl(&world, ":fibstep (R1R R2R + R3S pop R2R R1S pop R3R R2S pop R4R 1 + R4S pop rec)");
    yalex_repl(&world, ":rec (R4R R0R 1 + < 'fibstep _ select)");
    yalex_repl(&world, ":fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S rec pop pop pop pop pop pop R3R)");
    
             
                   

    char word[YALEX_SIZE_REPL_STR];
    while (1) {
        word[0] = 0;
        fgets(word, sizeof(word), stdin);
        yalex_repl(&world, word);
    }
    return 0;
}

int main(void) {
    yalex();
    return 0;
}
