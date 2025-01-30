#pragma once

#include "compiler/ast-tree.h"
#include <stddef.h>
#include <stdint.h>

typedef enum CodeGeneratorInstruction : uint8_t {
  CODE_GENERATOR_INSTRUCTION_PRINT_U64,
  CODE_GENERATOR_INSTRUCTION_CALL,
  CODE_GENERATOR_INSTRUCTION_RET,
  CODE_GENERATOR_INSTRUCTION_DEF_VAR64,
  CODE_GENERATOR_INSTRUCTION_MOV_U64,
} CodeGeneratorInstruction;

typedef struct CodeGeneratorCode {
  char *label_begin;
  char *label_end;
  CodeGeneratorInstruction instruction;
  void *metadata;
} CodeGeneratorCode;

typedef struct CodeGeneratorCall {
  char *label_begin;
  char *label_end;
} CodeGeneratorCall;

typedef struct CodeGeneratorOperand {
  char *value;
  bool isReference;
} CodeGeneratorOperand;

typedef struct CodeGeneratorDoubleOperand {
  CodeGeneratorOperand op0;
  CodeGeneratorOperand op1;
} CodeGeneratorDoubleOperand;

typedef struct CodeGeneratorCodes {
  CodeGeneratorCode *codes;
  size_t codes_size;
} CodeGeneratorCodes;

void codeGeneratorDelete(CodeGeneratorCodes *code);

CodeGeneratorOperand *newCodeGeneratorOperand(char *value, bool isReference);

CodeGeneratorOperand *makeCodeGeneratorOperand(AstTree tree);

CodeGeneratorCode createGenerateCode(char *label_begin, char *label_end,
                                     CodeGeneratorInstruction instruction,
                                     void *metadata);

CodeGeneratorCode *newGenerateCode(char *label_begin, char *label_end,
                                   CodeGeneratorInstruction instruction);

void generateCodePushCode(CodeGeneratorCodes *codes, CodeGeneratorCode code);

CodeGeneratorCodes *codeGenerator(AstTreeRoot *astTreeRoot);

bool codeGeneratorAstTreeFunction(char *label_begin, char *label_end,
                                  AstTree astTree, CodeGeneratorCodes *codes);

char *codeGeneratorToFlatASM(const CodeGeneratorCodes *codes);

bool codeGeneratorFlatASMExec(const char *filePath, const char *fasm);
