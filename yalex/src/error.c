#include "error.h"

void error_init(error *err) {
    err->code = NO_ERROR;
    err->token = 0;
}