#ifndef OPCODES_H
#define OPCODES_H

#include "vm.h"

void opcode_mov(VM *vm, struct _Instruction *instruction);
void opcode_add(VM *vm, struct _Instruction *instruction);
void opcode_sub(VM *vm, struct _Instruction *instruction);
void opcode_div(VM *vm, struct _Instruction *instruction);
void opcode_nop(VM *vm, struct _Instruction *instruction);
void opcode_xor(VM *vm, struct _Instruction *instruction);


#endif