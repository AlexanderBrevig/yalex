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
        if (err->program[err->loc] == ' '){
            tokenBegin++;
        }
        char currentChar = 0;
        do {
            currentChar = err->program[tokenBegin + (begin)];
            buf[begin] = currentChar;
            begin++;
        } while (currentChar != 0 && currentChar != ' ');
        buf[begin-1] = 0;
        tokenEnd = err->loc + begin -1;

        YALEXPRT();
        int arg = 0;
        switch (err->code){
            case NOT_A_NUMBER:
                arg = (int)err->token;
                break;
        }
        yalexPrint(buf);
        yalexPrint(" makes ");
        yalexPrint(errors[err->code]);
        yalexPrint(" at index ");
        itoa(tokenBegin, buf, 10);
        yalexPrint(buf);
        if (arg > 0) {
            yalexPrint(" for argument ");
            itoa(arg, buf, 10);
            yalexPrint(buf);
        }
        yalexPrintln();
        YALEXPRT();

        int16_t idx = 0;
        while(err->program[idx] != 0){
            if (idx == tokenBegin) {
                yalexPrint("{");
            }
            char buf[2] = " ";
            buf[0] = err->program[idx];
            yalexPrint(buf);
            if (idx == tokenEnd) {
                yalexPrint("}");
            }
            idx++;
        }
        yalexPrintln();
    }
    return err->code;
}