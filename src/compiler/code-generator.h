#pragma once

#include "compiler/ast-tree.h"
#include <stddef.h>
#include <stdint.h>

typedef enum CodeGeneratorInstruction : uint8_t {
  CODE_GENERATOR_INSTRUCTION_PRINT_U64,
  CODE_GENERATOR_INSTRUCTION_CALL,
  CODE_GENERATOR_INSTRUCTION_RET,
  CODE_GENERATOR_INSTRUCTION_MOV,
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
  union {
    uint64_t value;
    char *reference;
  } value;
  bool isReference;
} CodeGeneratorOperand;

typedef struct CodeGeneratorDoubleOperand {
  CodeGeneratorOperand op0;
  CodeGeneratorOperand op1;
} CodeGeneratorDoubleOperand;

typedef struct CodeGeneratorMov {
  CodeGeneratorOperand op0;
  CodeGeneratorOperand op1;
  size_t bytes;
} CodeGeneratorMov;

typedef struct CodeGeneratorDefine {
  char *label_begin;
  char *label_end;
  CodeGeneratorOperand operand;
  size_t bytes;
} CodeGeneratorDefine;

typedef struct CodeGeneratorCodes {
  CodeGeneratorCode *codes;
  size_t codes_size;
  CodeGeneratorDefine *defines;
  size_t defines_size;
} CodeGeneratorCodes;

void codeGeneratorOperandDestroy(CodeGeneratorOperand operand);
void codeGeneratorDelete(CodeGeneratorCodes *code);

CodeGeneratorOperand *newCodeGeneratorOperand(void *value, bool isReference);
CodeGeneratorOperand makeCodeGeneratorOperand(void *value, bool isReference);

CodeGeneratorOperand *newCodeGeneratorOperandFromAstTree(AstTree tree);

CodeGeneratorCode createGenerateCode(char *label_begin, char *label_end,
                                     CodeGeneratorInstruction instruction,
                                     void *metadata);

CodeGeneratorDefine createGenerateDefine(char *label_begin, char *label_end,
                                         CodeGeneratorOperand operand,
                                         size_t bytes);

CodeGeneratorCode *newGenerateCode(char *label_begin, char *label_end,
                                   CodeGeneratorInstruction instruction);

void generateCodePushCode(CodeGeneratorCodes *codes, CodeGeneratorCode code);
void generateCodePushDefine(CodeGeneratorCodes *codes,
                            CodeGeneratorDefine define);

CodeGeneratorCodes *codeGenerator(AstTreeRoot *astTreeRoot);

bool codeGeneratorAstTreeFunction(char *label_begin, char *label_end,
                                  AstTree astTree, CodeGeneratorCodes *codes);

char *codeGeneratorToFlatASM(const CodeGeneratorCodes *codes);

bool codeGeneratorFlatASMExec(const char *filePath, const char *fasm);
