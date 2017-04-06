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
        int tokenBegin = 0;
        int tokenEnd = err->loc;

        while(err->program[err->loc + (begin--)] != ' '){  }
        begin+=2;
        tokenBegin = err->loc + begin;

        uint16_t i = 0;
        while(err->program[err->loc + begin] != ' '){
            buf[i++] = err->program[err->loc + begin++];
        }
        buf[i] = 0;

        YALEXPRT();
        yalexPrint(buf);
        yalexPrint(" is ");
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
    }
    return err->code;
}