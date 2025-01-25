#include "code-generator.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void codeGeneratorDelete(CodeGeneratorCodes *code) {
  for (size_t i = 0; i < code->codes_size; ++i) {
    continue;
  }
  free(code->codes);
  free(code);
}

CodeGeneratorCode createGenerateCode(char *label_begin, char *label_end,
                                     CodeGeneratorInstruction instruction,
                                     void *metadata) {
  CodeGeneratorCode code = {
      .label_begin = label_begin,
      .label_end = label_end,
      .instruction = instruction,
      .metadata = metadata,
  };
  return code;
}

CodeGeneratorCode *newGenerateCode(char *label_begin, char *label_end,
                                   CodeGeneratorInstruction instruction) {
  CodeGeneratorCode *result = a404m_malloc(sizeof(*result));
  result->label_begin = label_begin;
  result->label_end = label_end;
  result->instruction = instruction;
  return result;
}

void generateCodePushCode(CodeGeneratorCodes *codes, CodeGeneratorCode code) {
  size_t codes_size =
      a404m_malloc_usable_size(codes->codes) / sizeof(*codes->codes);
  if (codes_size == codes->codes_size) {
    codes_size += codes_size / 2 + 1;
    codes->codes =
        a404m_realloc(codes->codes, codes_size * sizeof(*codes->codes));
  }
  codes->codes[codes->codes_size] = code;
  codes->codes_size += 1;
}

CodeGeneratorCodes *codeGenerator(AstTreeRoot *astTreeRoot) {
  CodeGeneratorCodes *codes = a404m_malloc(sizeof(*codes));

  codes->codes = a404m_malloc(0);
  codes->codes_size = 0;

  for (size_t i = 0; i < astTreeRoot->variables.size; ++i) {
    AstTreeVariable *variable = astTreeRoot->variables.data[i];
    switch (variable->value->token) {
    case AST_TREE_TOKEN_FUNCTION:
      if (!codeGeneratorAstTreeFunction(variable->name_begin,
                                        variable->name_end, *variable->value,
                                        codes)) {
        return NULL;
      }
      continue;
    case AST_TREE_TOKEN_KEYWORD_PRINT:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_NONE:
    }
    printLog("Bad token %d", variable->value->token);
    return NULL;
  }

  return codes;
}

bool codeGeneratorAstTreeFunction(char *label_begin, char *label_end,
                                  AstTree astTree, CodeGeneratorCodes *codes) {
  AstTreeFunction *metadata = astTree.metadata;
  AstTreeScope scope = metadata->scope;

  for (size_t i = 0; i < scope.expressions_size; ++i) {
    AstTree tree = scope.expressions[i];
    switch (tree.token) {
    case AST_TREE_TOKEN_KEYWORD_PRINT:
      generateCodePushCode(
          codes, createGenerateCode(label_begin, label_end,
                                    CODE_GENERATOR_INSTRUCTION_PRINT, NULL));
      goto OK;
    case AST_TREE_TOKEN_FUNCTION_CALL: {
      AstTreeFunctionCall *metadata = tree.metadata;
      AstTree *function = metadata->function;
      if (metadata->parameters_size != 0) {
        printLog("Not implemented");
        exit(0);
      }
      if (function->token != AST_TREE_TOKEN_IDENTIFIER) {
        printLog("Not implemented");
        exit(0);
      }
      AstTreeIdentifier *function_metadata = function->metadata;
      CodeGeneratorCall *callMetadata = a404m_malloc(sizeof(*callMetadata));
      callMetadata->label_begin = function_metadata->name_begin;
      callMetadata->label_end = function_metadata->name_end;
      generateCodePushCode(codes,
                           createGenerateCode(label_begin, label_end,
                                              CODE_GENERATOR_INSTRUCTION_CALL,
                                              callMetadata));
    }
      goto OK;
    case AST_TREE_TOKEN_IDENTIFIER:
    case AST_TREE_TOKEN_FUNCTION:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_NONE:
    }
    printLog("Bad token %d", tree.token);
    return false;
  OK:
    label_begin = NULL;
    label_end = NULL;
  }

  generateCodePushCode(codes, createGenerateCode(label_begin, label_end,
                                                 CODE_GENERATOR_INSTRUCTION_RET,
                                                 NULL));

  return true;
}

static const char TEMPLATE[] =
    "format ELF64 executable 3\nSYS_exit = 60\nSYS_write = 1\nSTDOUT = "
    "1\nsegment readable writable\nhello: db \"Hello, "
    "World!\",0xa\nhello_len = $-hello\nsegment readable executable\nentry "
    "_start\nprint:\nmov rax, SYS_write\nmov rdi, STDOUT\nmov rsi, "
    "hello\nmov rdx, hello_len\nsyscall\nret\n_start:\ncall main\nmov rax, "
    "SYS_exit\nxor "
    "rdi,rdi\nsyscall\n";
static const size_t TEMPLATE_LEN =
    sizeof(TEMPLATE) / sizeof(*TEMPLATE) - sizeof(*TEMPLATE);

static void codeGeneratorAppendFlatASMCommand(char **fasm, size_t *fasm_size,
                                              size_t *fasm_inserted,
                                              const char *str, size_t str_len) {
  size_t inserted_last = *fasm_inserted;
  *fasm_inserted += str_len;
  if (*fasm_inserted + 1 >= *fasm_size) {
    *fasm_size = *fasm_inserted + *fasm_inserted / 2 + 1;
    *fasm = a404m_realloc(*fasm, *fasm_size * sizeof(*fasm));
  }
  strncpy(*fasm + inserted_last, str, str_len);
}

char *codeGeneratorToFlatASM(const CodeGeneratorCodes *codes) {
  size_t fasm_size = TEMPLATE_LEN + 1;
  char *fasm = a404m_malloc(fasm_size * sizeof(*fasm));
  size_t fasm_inserted = 0;

  codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, TEMPLATE,
                                    TEMPLATE_LEN);

  for (size_t i = 0; i < codes->codes_size; ++i) {
    const CodeGeneratorCode code = codes->codes[i];
    if (code.label_begin != code.label_end) {
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted,
                                        code.label_begin,
                                        code.label_end - code.label_begin);

      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted,
                                        ":\n", 2);
    }

    switch (code.instruction) {
    case CODE_GENERATOR_INSTRUCTION_PRINT: {
      constexpr char INST[] = "call print\n";
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, INST,
                                        strlen(INST));
    }
      continue;
    case CODE_GENERATOR_INSTRUCTION_CALL: {
      CodeGeneratorCall *metadata = code.metadata;
      constexpr char CALL_INST[] = "call ";
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted,
                                        CALL_INST, strlen(CALL_INST));
      codeGeneratorAppendFlatASMCommand(
          &fasm, &fasm_size, &fasm_inserted, metadata->label_begin,
          metadata->label_end - metadata->label_begin);
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, "\n",
                                        1);
    }
      continue;
    case CODE_GENERATOR_INSTRUCTION_RET: {
      constexpr char INST[] = "ret\n";
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, INST,
                                        strlen(INST));
    }
      continue;
    }
    printLog("Bad instruction %d", code.instruction);
  }

  fasm[fasm_inserted] = '\0';
  return fasm;
}

bool codeGeneratorFlatASMExec(const char *filePath, const char *fasm) {
  const size_t filePathLen = strlen(filePath);
  char *asmFilePath = a404m_malloc((filePathLen + 4 + 1) * sizeof(char));
  strncpy(asmFilePath, filePath, filePathLen);
  strcpy(asmFilePath + filePathLen, ".asm");

  FILE *file = fopen(asmFilePath, "w");

  if (file == NULL) {
    free(asmFilePath);
    return false;
  }

  fwrite(fasm, sizeof(*fasm), strlen(fasm), file);

  fclose(file);

  char *command;
  asprintf(&command, "fasm -m 102400 \"%s\" \"%s\"", asmFilePath, filePath);
  free(asmFilePath);

  if (system(command) != 0) {
    free(command);
    return false;
  }
  free(command);

  asprintf(&command, "chmod + \"%s\"", filePath);

  if (system(command) != 0) {
    free(command);
    return false;
  }
  free(command);

  return true;
}
