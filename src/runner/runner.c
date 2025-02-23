#include "runner.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define doOperation(op0, op1, operator, originalType, type)                    \
  (op0)->metadata = (void *)(u64)((type)(originalType)(op0)                    \
                                      ->metadata                               \
                                      operator(type)(originalType)(op1)        \
                                      ->metadata)

#define doOperationFloat(op0, op1, operator, originalType, type)               \
  *((originalType *)(op0)->metadata) =                                         \
      (originalType)((type) * (originalType *)(op0)->metadata operator(type) * \
                     (originalType *)(op1)->metadata)

#define doLogicalOperation(op0, op1, operator, originalType, _type)            \
  {                                                                            \
    bool res = (bool)(((_type)(originalType)(op0)->metadata) operator(         \
        (_type)(originalType)(op1)->metadata));                                \
    astTreeDestroy(*(op0));                                                    \
    (op0)->metadata = (void *)(bool)res;                                       \
    (op0)->type = &AST_TREE_BOOL_TYPE;                                         \
    (op0)->token = AST_TREE_TOKEN_VALUE_BOOL;                                  \
  }

#define doLogicalOperationFloat(op0, op1, operator, originalType, _type)       \
  {                                                                            \
    bool res = (bool)(((_type) * ((originalType *)(op0)->metadata)) operator(  \
        (_type) * ((originalType *)(op1)->metadata)));                         \
    astTreeDestroy(*(op0));                                                    \
    (op0)->metadata = (void *)(bool)res;                                       \
    (op0)->type = &AST_TREE_BOOL_TYPE;                                         \
    (op0)->token = AST_TREE_TOKEN_VALUE_BOOL;                                  \
  }

#define doLeftOperation(op0, operator, originalType, type)                     \
  (op0)->metadata = (void *)(u64)(operator(type)(originalType)(op0)->metadata)

#define doLeftOperationFloat(op0, operator, originalType, type)                \
  *((originalType *)(op0)->metadata) = operator(                               \
      (type) * (originalType *)(op0)->metadata)

void runnerVariablesDelete(RunnerVariables *variables) {
  for (size_t i = 0; i < variables->size; ++i) {
    if (variables->data[i]->value != NULL) {
      astTreeDelete(variables->data[i]->value);
    }
    free(variables->data[i]);
  }
  free(variables->data);
  free(variables);
}

void runnerVariablePush(RunnerVariables *variables, AstTreeVariable *variable) {
  size_t variables_size =
      a404m_malloc_usable_size(variables->data) / sizeof(*variables->data);
  if (variables_size == variables->size) {
    variables_size += variables_size / 2 + 1;
    variables->data = a404m_realloc(variables->data,
                                    variables_size * sizeof(*variables->data));
  }
  variables->data[variables->size] =
      a404m_malloc(sizeof(*variables->data[variables->size]));
  variables->data[variables->size]->variable = variable;
  variables->data[variables->size]->value = NULL;
  variables->size += 1;
}

void runnerVariableSetValue(RunnerVariablePages *pages,
                            AstTreeVariable *variable, AstTree *value) {
  if (variable->isConst) {
    printError(variable->name_begin, variable->name_end,
               "Can't assign to const");
    exit(1);
  }
  for (size_t i = 0; i < pages->size; ++i) {
    RunnerVariables *variables = pages->data[i];
    for (size_t j = 0; j < variables->size; ++j) {
      RunnerVariable *var = variables->data[j];
      if (var->variable == variable) {
        if (var->value != NULL) {
          astTreeDelete(var->value);
        }
        var->value = value;
        return;
      }
    }
  }

  printError(variable->name_begin, variable->name_end, "Variable not found");
  UNREACHABLE;
}

AstTree *runnerVariableGetValue(RunnerVariablePages *pages,
                                AstTreeVariable *variable) {
  if (variable->isConst) {
    return variable->value;
  }
  for (size_t i = 0; i < pages->size; ++i) {
    RunnerVariables *variables = pages->data[i];
    for (size_t j = 0; j < variables->size; ++j) {
      RunnerVariable *var = variables->data[j];
      if (var->variable == variable) {
        if (var->value != NULL) {
          return var->value;
        } else {
          return var->variable->value;
        }
      }
    }
  }

  printError(variable->name_begin, variable->name_end, "Variable not found");
  UNREACHABLE;
}

RunnerVariablePages initRootPages() {
  RunnerVariablePages pages = {
      .data = a404m_malloc(sizeof(*pages.data)),
      .size = 1,
  };

  pages.data[pages.size - 1] =
      a404m_malloc(sizeof(*pages.data[pages.size - 1]));
  pages.data[pages.size - 1]->data = a404m_malloc(0);
  pages.data[pages.size - 1]->size = 0;

  return pages;
}

void destroyRootPages(RunnerVariablePages pages) {
  runnerVariablesDelete(pages.data[pages.size - 1]);
  free(pages.data);
}

bool runAstTree(AstTreeRoot *root) {
  constexpr char MAIN_STR[] = "main";
  constexpr size_t MAIN_STR_SIZE =
      (sizeof(MAIN_STR) / sizeof(*MAIN_STR)) - sizeof(*MAIN_STR);

  RunnerVariablePages pages = initRootPages();

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (variable->isConst) {
      runnerVariablePush(pages.data[pages.size - 1], variable);
    }
  }

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    AstTree *variable_value = runnerVariableGetValue(&pages, variable);
    size_t name_size = variable->name_end - variable->name_begin;
    if (name_size == MAIN_STR_SIZE &&
        strncmp(variable->name_begin, MAIN_STR, MAIN_STR_SIZE) == 0 &&
        variable_value->token == AST_TREE_TOKEN_FUNCTION) {

      AstTree *main = variable_value;

      const bool ret =
          runAstTreeFunction(main, NULL, 0, &pages) == &AST_TREE_VOID_VALUE;
      destroyRootPages(pages);
      return ret;
    }
  }
  destroyRootPages(pages);
  printLog("main function is not found");
  return false;
}

AstTree *runAstTreeFunction(AstTree *tree, AstTree **arguments,
                            size_t arguments_size,
                            RunnerVariablePages *_pages) {
  AstTreeFunction *function = tree->metadata;

  RunnerVariablePages pages = {
      .data = a404m_malloc((_pages->size + 1) * sizeof(*pages.data)),
      .size = _pages->size + 1,
  };

  for (size_t i = 0; i < _pages->size; ++i) {
    pages.data[i] = _pages->data[i];
  }

  pages.data[pages.size - 1] =
      a404m_malloc(sizeof(*pages.data[pages.size - 1]));
  pages.data[pages.size - 1]->data = a404m_malloc(0);
  pages.data[pages.size - 1]->size = 0;

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTreeVariable *variable = function->arguments.data[i];
    AstTree *value = runExpression(arguments[i], &pages);
    runnerVariablePush(pages.data[pages.size - 1], variable);
    runnerVariableSetValue(&pages, variable, value);
  }

  for (size_t i = arguments_size; i < function->arguments.size; ++i) {
    AstTreeVariable *variable = function->arguments.data[i];
    runnerVariablePush(pages.data[pages.size - 1], variable);
  }

  for (size_t i = 0; i < function->scope.variables.size; ++i) {
    AstTreeVariable *variable = function->scope.variables.data[i];
    runnerVariablePush(pages.data[pages.size - 1], variable);
  }

  AstTree *ret = &AST_TREE_VOID_VALUE;

  for (size_t i = 0; i < function->scope.expressions_size; ++i) {
    AstTree *r = runExpression(function->scope.expressions[i], &pages);
    if (r) {
      ret = r;
      break;
    }
  }

  runnerVariablesDelete(pages.data[pages.size - 1]);
  free(pages.data);

  return ret;
}

AstTree *runExpression(AstTree *expr, RunnerVariablePages *pages) {
  switch (expr->token) {
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTree *tree = runExpression(metadata, pages);
    printf("%lu", (AstTreeInt)tree->metadata);
    astTreeDelete(tree);
  }
    return NULL;
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = expr->metadata;
    if (metadata->function->token == AST_TREE_TOKEN_VARIABLE) {
      AstTreeVariable *variable = metadata->function->metadata;
      return runAstTreeFunction(runnerVariableGetValue(pages, variable),
                                metadata->parameters, metadata->parameters_size,
                                pages);
    } else {
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreeInfix *metadata = expr->metadata;
    if (metadata->left.token == AST_TREE_TOKEN_VARIABLE) {
      AstTreeVariable *left = metadata->left.metadata;
      runnerVariableSetValue(pages, left,
                             runExpression(&metadata->right, pages));
    } else {
      UNREACHABLE;
    }
  }
    return NULL;
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = expr->metadata;
    if (metadata->value != NULL) {
      return runExpression(metadata->value, pages);
    } else {
      return &AST_TREE_VOID_VALUE;
    }
  }
  case AST_TREE_TOKEN_VARIABLE_DEFINE: {
    AstTreeVariable *variable = expr->metadata;
    runnerVariableSetValue(pages, variable, copyAstTree(variable->value));
  }
    return NULL;
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = expr->metadata;
    AstTree *condition = runExpression(metadata->condition, pages);
    AstTree *ret;
    if ((AstTreeBool)condition->metadata) {
      ret = runExpression(metadata->ifBody, pages);
    } else if (metadata->elseBody != NULL) {
      ret = runExpression(metadata->elseBody, pages);
    } else {
      ret = NULL;
    }
    astTreeDelete(condition);
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = expr->metadata;
    AstTree *ret = NULL;
    while (true) {
      AstTree *tree = runExpression(metadata->condition, pages);
      bool conti = (AstTreeBool)tree->metadata;
      astTreeDelete(tree);
      if (!conti) {
        break;
      }
      ret = runExpression(metadata->body, pages);
    }
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *operand = expr->metadata;
    return runExpression((AstTree *)operand, pages);
  }
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = expr->metadata;

    RunnerVariablePages newPages = {
        .data = a404m_malloc((pages->size + 1) * sizeof(*newPages.data)),
        .size = pages->size + 1,
    };

    for (size_t i = 0; i < pages->size; ++i) {
      newPages.data[i] = pages->data[i];
    }

    newPages.data[pages->size] =
        a404m_malloc(sizeof(*newPages.data[pages->size]));
    newPages.data[pages->size]->size = 0;
    newPages.data[pages->size]->data =
        a404m_malloc(newPages.data[pages->size]->size *
                     sizeof(*newPages.data[pages->size]->data));

    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *variable = metadata->variables.data[i];
      runnerVariablePush(newPages.data[newPages.size - 1], variable);
    }

    AstTree *ret = NULL;
    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      ret = runExpression(metadata->expressions[i], &newPages);
    }
    for (size_t i = 0; i < newPages.data[pages->size]->size; ++i) {
      if (newPages.data[pages->size]->data[i]->value) {
        astTreeDelete(newPages.data[pages->size]->data[i]->value);
      }
      free(newPages.data[pages->size]->data[i]);
    }
    free(newPages.data[pages->size]->data);
    free(newPages.data[pages->size]);
    free(newPages.data);
    return ret;
  }
  case AST_TREE_TOKEN_OPERATOR_PLUS: {
    AstTreeSingleChild *operand = runExpression(expr->metadata, pages);
    if (operand->type == &AST_TREE_U64_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, u64);
    } else if (operand->type == &AST_TREE_I64_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, i64);
    } else if (operand->type == &AST_TREE_U32_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, u32);
    } else if (operand->type == &AST_TREE_I32_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, i32);
    } else if (operand->type == &AST_TREE_U16_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, u16);
    } else if (operand->type == &AST_TREE_I16_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, i16);
    } else if (operand->type == &AST_TREE_U8_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, u8);
    } else if (operand->type == &AST_TREE_I8_TYPE) {
      doLeftOperation(operand, +, AstTreeInt, i8);
    } else if (operand->type == &AST_TREE_F128_TYPE) {
      doLeftOperationFloat(operand, +, AstTreeFloat, f128);
    } else if (operand->type == &AST_TREE_F64_TYPE) {
      doLeftOperationFloat(operand, +, AstTreeFloat, f64);
    } else if (operand->type == &AST_TREE_F32_TYPE) {
      doLeftOperationFloat(operand, +, AstTreeFloat, f32);
    } else if (operand->type == &AST_TREE_F16_TYPE) {
      doLeftOperationFloat(operand, +, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeSingleChild *operand = runExpression(expr->metadata, pages);
    if (operand->type == &AST_TREE_U64_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, u64);
    } else if (operand->type == &AST_TREE_I64_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, i64);
    } else if (operand->type == &AST_TREE_U32_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, u32);
    } else if (operand->type == &AST_TREE_I32_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, i32);
    } else if (operand->type == &AST_TREE_U16_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, u16);
    } else if (operand->type == &AST_TREE_I16_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, i16);
    } else if (operand->type == &AST_TREE_U8_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, u8);
    } else if (operand->type == &AST_TREE_I8_TYPE) {
      doLeftOperation(operand, -, AstTreeInt, i8);
    } else if (operand->type == &AST_TREE_F128_TYPE) {
      doLeftOperationFloat(operand, -, AstTreeFloat, f128);
    } else if (operand->type == &AST_TREE_F64_TYPE) {
      doLeftOperationFloat(operand, -, AstTreeFloat, f64);
    } else if (operand->type == &AST_TREE_F32_TYPE) {
      doLeftOperationFloat(operand, -, AstTreeFloat, f32);
    } else if (operand->type == &AST_TREE_F16_TYPE) {
      doLeftOperationFloat(operand, -, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_SUM: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, +, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, +, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, +, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, +, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, +, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, +, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, +, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, +, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, +, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, +, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, +, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, +, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SUB: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, -, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, -, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, -, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, -, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, -, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, -, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, -, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, -, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, -, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, -, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, -, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, -, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, *, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, *, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, *, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, *, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, *, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, *, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, *, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, *, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, *, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, *, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, *, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, *, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_DIVIDE: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, /, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, /, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, /, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, /, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, /, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, /, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, /, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, /, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, /, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, /, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, /, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, /, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_MODULO: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, %, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, %, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, %, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, %, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, %, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, %, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, %, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, %, AstTreeInt, i8);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, ==, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, ==, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, ==, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, ==, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, ==, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, ==, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, ==, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, ==, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, ==, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, ==, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, ==, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, ==, AstTreeFloat, f16);
    } else if (left->type == &AST_TREE_TYPE_TYPE) {
      bool res = typeIsEqual(left, right);
      astTreeDelete(left);
      left = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, (void *)(AstTreeBool)res,
                        &AST_TREE_BOOL_TYPE, NULL, NULL);
      left->token = AST_TREE_TOKEN_VALUE_BOOL;
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, !=, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, !=, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, !=, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, !=, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, !=, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, !=, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, !=, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, !=, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, !=, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, !=, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, !=, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, !=, AstTreeFloat, f16);
    } else if (left->type == &AST_TREE_TYPE_TYPE) {
      bool res = !typeIsEqual(left, right);
      astTreeDelete(left);
      left = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, (void *)(AstTreeBool)res,
                        &AST_TREE_BOOL_TYPE, NULL, NULL);
      left->token = AST_TREE_TOKEN_VALUE_BOOL;
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_GREATER: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, >, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, >, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, >, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, >, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, >, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, >, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, >, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, >, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, >, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, >, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, >, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, >, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SMALLER: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, <, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, <, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, <, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, <, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, <, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, <, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, <, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, <, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, <, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, <, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, <, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, <, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, >=, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, >=, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, >=, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, >=, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, >=, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, >=, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, >=, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, >=, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, >=, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, >=, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, >=, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, >=, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, pages);
    AstTree *right = runExpression(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doOperation(left, right, <=, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doOperation(left, right, <=, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doOperation(left, right, <=, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doOperation(left, right, <=, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doOperation(left, right, <=, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doOperation(left, right, <=, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doOperation(left, right, <=, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doOperation(left, right, <=, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doOperationFloat(left, right, <=, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperationFloat(left, right, <=, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperationFloat(left, right, <=, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperationFloat(left, right, <=, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_FLOAT:
    return copyAstTree(expr);
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = expr->metadata;
    AstTree *value = runnerVariableGetValue(pages, variable);
    return runExpression(value, pages);
  }
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}
