#include <stdio.h>
#include <string.h>

#include "yalex.h"

void replMessageCallback(const char* ptr) {
    if (ptr && strlen(ptr) > 0) { printf("%s\n", ptr); }
}
#define SIZEOF(item) printf("Size of %s: %d bytes\n", #item, sizeof(item))
int yalex(void) {
    yalex_world world;
    yalex_init(&world, replMessageCallback);

    SIZEOF(world);
    SIZEOF(world.stack[0]);
    SIZEOF(world.lambdas[0]);

    replMessageCallback("yalex 1.0beta");

    yalex_repl(&world, ":fibstep (R1R R2R + R3S pop R2R R1S pop R3R R2S pop R4R 1 + R4S pop rec)");
    yalex_repl(&world, ":rec (R4R R0R 1 + < 'fibstep _ select)");
    yalex_repl(&world, ":start (R0R 1 - R0S pop rec pop pop pop pop pop pop R3R)");
    yalex_repl(&world, ":fib (R0S 0 R1S 1 R2S 0 R3S 1 R4S R0R 3 < 1 'start select)");
    
    char word[128];
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
