#pragma once

#include "compiler/ast-tree.h"
#include <stddef.h>
#include <stdint.h>

typedef enum CodeGeneratorInstruction : uint8_t {
  CODE_GENERATOR_INSTRUCTION_PRINT,
  CODE_GENERATOR_INSTRUCTION_RET,
} CodeGeneratorInstruction;

typedef struct CodeGeneratorCode {
  char *label_begin;
  char *label_end;
  CodeGeneratorInstruction instruction;
} CodeGeneratorCode;

typedef struct CodeGeneratorCodes {
  CodeGeneratorCode *codes;
  size_t codes_size;
} CodeGeneratorCodes;

CodeGeneratorCode createGenerateCode(char *label_begin, char *label_end,
                                   CodeGeneratorInstruction instruction);

CodeGeneratorCode *newGenerateCode(char *label_begin, char *label_end,
                                   CodeGeneratorInstruction instruction);

void generateCodePushCode(CodeGeneratorCodes *codes, CodeGeneratorCode code);

CodeGeneratorCodes *codeGenerator(AstTreeRoot *astTreeRoot);

bool codeGeneratorAstTreeFunction(char *label_begin,char *label_end,AstTree astTree, CodeGeneratorCodes *codes);

char *codeGeneratorToFlatASM(const CodeGeneratorCodes *codes);

bool codeGeneratorFlatASMExec(const char *filePath,const char *fasm);
