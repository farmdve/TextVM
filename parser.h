#ifndef PARSER_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <string.h>



int INS_op(VM *vm, char *buf, int instr);
int INS_Split(VM *vm, char *buf, int pass, int instr);
int countComma(const char *buf);
int INS_parse(VM *vm, const char *disasm);

#endif