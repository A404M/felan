#include "code-generator.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void codeGeneratorDelete(CodeGeneratorCodes *code) {
  for (size_t i = 0; i < code->codes_size; ++i) {
    CodeGeneratorCode current = code->codes[i];
    switch (current.instruction) {
    case CODE_GENERATOR_INSTRUCTION_PRINT_U64:
    case CODE_GENERATOR_INSTRUCTION_DEF_VAR64: {
      CodeGeneratorOperand *metadata = current.metadata;
      free(metadata);
    }
    case CODE_GENERATOR_INSTRUCTION_RET:
      continue;
    case CODE_GENERATOR_INSTRUCTION_CALL: {
      CodeGeneratorCall *metadata = current.metadata;
      free(metadata);
    }
      continue;
    case CODE_GENERATOR_INSTRUCTION_MOV_U64: {
      CodeGeneratorDoubleOperand *metadata = current.metadata;
      free(metadata->op0.value);
      free(metadata->op1.value);
      free(metadata);
    }
      continue;
    }
    printLog("Bad instruction %d", current.instruction);
    exit(1);
  }
  free(code->codes);
  free(code);
}

CodeGeneratorOperand *newCodeGeneratorOperand(char *value, bool isReference) {
  CodeGeneratorOperand *result = a404m_malloc(sizeof(*result));

  result->value = value;
  result->isReference = isReference;

  return result;
}

CodeGeneratorOperand *makeCodeGeneratorOperand(AstTree tree) {
  switch (tree.token) {
  case AST_TREE_TOKEN_VALUE_U64:
    return newCodeGeneratorOperand(u64ToString((AstTreeU64)tree.metadata),
                                   false);
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = tree.metadata;
    if (!typeIsEqual(variable->type, &AST_TREE_U64_TYPE)) {
      UNREACHABLE;
    }
    if (variable->isConst) {
      return newCodeGeneratorOperand(
          u64ToString((AstTreeU64)variable->value->metadata), false);
    } else {
      char *name = a404m_malloc(
          (variable->name_end - variable->name_begin + 1) * sizeof(*name));
      strncpy(name, variable->name_begin,
              variable->name_end - variable->name_begin);
      name[variable->name_end - variable->name_begin] = '\0';

      return newCodeGeneratorOperand(name, true);
    }
  }
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
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
    case AST_TREE_TOKEN_VALUE_U64:
      if (!variable->isConst) {
        CodeGeneratorOperand *value = newCodeGeneratorOperand(
            u64ToString((AstTreeU64)variable->value->metadata), false);
        generateCodePushCode(
            codes,
            createGenerateCode(variable->name_begin, variable->name_end,
                               CODE_GENERATOR_INSTRUCTION_DEF_VAR64, value));
      }
      continue;
    case AST_TREE_TOKEN_TYPE_TYPE:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_VARIABLE:
    case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
    case AST_TREE_TOKEN_FUNCTION_CALL:
    case AST_TREE_TOKEN_TYPE_U64:
    case AST_TREE_TOKEN_VARIABLE_DEFINE:
    case AST_TREE_TOKEN_NONE:
      break;
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
    case AST_TREE_TOKEN_FUNCTION_CALL: {
      AstTreeFunctionCall *metadata = tree.metadata;
      AstTree *function = metadata->function;
      if (metadata->parameters_size != 0) {
        printLog("Not implemented");
        exit(0);
      }
      if (function->token != AST_TREE_TOKEN_VARIABLE) {
        printLog("Not implemented");
        exit(0);
      }
      AstTreeVariable *function_metadata = function->metadata;
      CodeGeneratorCall *callMetadata = a404m_malloc(sizeof(*callMetadata));
      callMetadata->label_begin = function_metadata->name_begin;
      callMetadata->label_end = function_metadata->name_end;
      generateCodePushCode(codes,
                           createGenerateCode(label_begin, label_end,
                                              CODE_GENERATOR_INSTRUCTION_CALL,
                                              callMetadata));
    }
      goto OK;
    case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
      AstTreeSingleChild *metadata = tree.metadata;
      if (metadata->token == AST_TREE_TOKEN_VALUE_U64) {
        CodeGeneratorOperand *value = newCodeGeneratorOperand(
            u64ToString((AstTreeU64)metadata->metadata), false);
        generateCodePushCode(
            codes,
            createGenerateCode(label_begin, label_end,
                               CODE_GENERATOR_INSTRUCTION_PRINT_U64, value));
      } else if (metadata->token == AST_TREE_TOKEN_VARIABLE) {
        AstTreeVariable *variable = metadata->metadata;
        if (variable->isConst) {
          CodeGeneratorOperand *value = newCodeGeneratorOperand(
              u64ToString((AstTreeU64)variable->value->metadata), false);
          generateCodePushCode(
              codes,
              createGenerateCode(label_begin, label_end,
                                 CODE_GENERATOR_INSTRUCTION_PRINT_U64, value));
        } else {
          char *name = a404m_malloc(
              (variable->name_end - variable->name_begin + 1) * sizeof(*name));
          strncpy(name, variable->name_begin,
                  variable->name_end - variable->name_begin);
          name[variable->name_end - variable->name_begin] = '\0';
          generateCodePushCode(
              codes, createGenerateCode(label_begin, label_end,
                                        CODE_GENERATOR_INSTRUCTION_PRINT_U64,
                                        newCodeGeneratorOperand(name, true)));
        }
      } else {
        printLog("Not implemented yet");
        exit(1);
      }
    }
      goto OK;
    case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
      AstTreeInfix *infix = tree.metadata;

      if (infix->left.token != AST_TREE_TOKEN_VARIABLE) {
        printLog("Not implemented yet");
        exit(1);
      } else if (infix->leftType.token != AST_TREE_TOKEN_TYPE_U64) {
        printLog("Not implemented yet");
        exit(1);
      }

      CodeGeneratorDoubleOperand *operands = a404m_malloc(sizeof(*operands));
      CodeGeneratorOperand *op0 = makeCodeGeneratorOperand(infix->left);
      CodeGeneratorOperand *op1 = makeCodeGeneratorOperand(infix->right);
      operands->op0 = *op0;
      operands->op1 = *op1;
      free(op0);
      free(op1);

      generateCodePushCode(
          codes,
          createGenerateCode(label_begin, label_end,
                             CODE_GENERATOR_INSTRUCTION_MOV_U64, operands));
    }
      goto OK;
    case AST_TREE_TOKEN_VARIABLE_DEFINE:
    case AST_TREE_TOKEN_VALUE_U64:
    case AST_TREE_TOKEN_VARIABLE:
    case AST_TREE_TOKEN_FUNCTION:
    case AST_TREE_TOKEN_TYPE_TYPE:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_TYPE_U64:
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

static const char TEMPLATE[] = "include 'stdlib/main.asm'\n";

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
    case CODE_GENERATOR_INSTRUCTION_PRINT_U64: {
      CodeGeneratorOperand *metadata = code.metadata;
      char *inst;
      if (metadata->isReference) {
        asprintf(&inst, "mov rdi,[%s]\ncall print_u64\n", metadata->value);
      } else {
        asprintf(&inst, "mov rdi,%s\ncall print_u64\n", metadata->value);
      }
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, inst,
                                        strlen(inst));
      free(inst);
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
    case CODE_GENERATOR_INSTRUCTION_DEF_VAR64: {
      CodeGeneratorOperand *metadata = code.metadata;
      char *inst;
      if (metadata->isReference) {
        asprintf(&inst, "db [%s]\n", metadata->value);
      } else {
        asprintf(&inst, "db %s\n", metadata->value);
      }
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, inst,
                                        strlen(inst));
      free(inst);
    }
      continue;
    case CODE_GENERATOR_INSTRUCTION_MOV_U64: {
      CodeGeneratorDoubleOperand *metadata = code.metadata;
      char *inst;
      if (metadata->op1.isReference) {
        asprintf(&inst, "mov rax, [%s]\n", metadata->op1.value);
      } else {
        asprintf(&inst, "mov rax, %s\n", metadata->op1.value);
      }
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, inst,
                                        strlen(inst));
      free(inst);
      if (metadata->op0.isReference) {
        asprintf(&inst, "mov [%s], rax\n", metadata->op0.value);
      } else {
        UNREACHABLE;
      }
      codeGeneratorAppendFlatASMCommand(&fasm, &fasm_size, &fasm_inserted, inst,
                                        strlen(inst));
      free(inst);
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
