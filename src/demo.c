#include <stdio.h>
#include <string.h>

#include "yalex.h"

void replMessageCallback(const char* ptr) {
    if (ptr && strlen(ptr) > 0) { printf("%s\n", ptr); }
}
#define SIZEOF(item) printf("Size of %s: %ud bytes\n", #item, (unsigned int) sizeof(item))
int yalex(void) {
    yalex_world world;
    yalex_init(&world, replMessageCallback);

    SIZEOF(world);
    SIZEOF(world.stack[0]);
    SIZEOF(world.lambdas[0]);

    replMessageCallback("yalex 1.0beta");

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
