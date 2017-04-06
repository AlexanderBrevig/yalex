#include <stdlib.h>
#include "error.h"
#include "yalex_util.h"

void error_init(error *err) {
    err->code = NO_ERROR;
    err->token = 0;
}

errorcode error_print(error *err){
    if (err->code != NO_ERROR) {
        char buf[128];
        int begin = 0;
        int tokenEnd = 0;
        int tokenBegin = err->loc;
        char currentChar = 0;
        do {
            currentChar = err->program[err->loc + (begin)];
            buf[begin] = currentChar;
            begin++;
        } while (currentChar != 0 && currentChar != ' ');

        tokenEnd = err->loc + begin;

        YALEXPRT();
        yalexPrint(buf);
        yalexPrint(" makes ");
        yalexPrint(errors[err->code]);
        itoa(tokenBegin, buf, 10);
        yalexPrint(" at index ");
        yalexPrint(buf);
        yalexPrintln();
        YALEXPRT();

        int16_t idx = 0;
        while(err->program[idx] != 0){
            if (idx == tokenBegin) {
                yalexPrint("#");
            }
            char buf[2] = " ";
            buf[0] = err->program[idx];
            yalexPrint(buf);
            if (idx == tokenEnd) {
                yalexPrint("#");
            }
            idx++;
        }
        yalexPrintln();
    }
    return err->code;
}