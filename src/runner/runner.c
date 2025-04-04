#include "runner.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define doOperation(op0, op1, operator, originalType, type)                    \
  *((originalType *)(op0)->metadata) =                                         \
      (originalType)(((type) * (originalType *)(op0)->metadata) operator(      \
          (type) * (originalType *)(op1)->metadata))

#define doLogicalOperation(op0, op1, operator, originalType, _type)            \
  {                                                                            \
    bool *res = a404m_malloc(sizeof(*res));                                    \
    *res = ((_type) * ((originalType *)(op0)->metadata)) operator(             \
        (_type) * ((originalType *)(op1)->metadata));                          \
    astTreeDestroy(*(op0));                                                    \
    (op0)->metadata = res;                                                     \
    (op0)->type = &AST_TREE_BOOL_TYPE;                                         \
    (op0)->token = AST_TREE_TOKEN_VALUE_BOOL;                                  \
  }

#define doLeftOperation(op0, operator, originalType, type)                     \
  *((originalType *)(op0)->metadata) = operator(                               \
      (type) * (originalType *)(op0)->metadata)

void runnerVariableSetValue(AstTreeVariable *variable, AstTree *value) {
  if (variable->isConst) {
    UNREACHABLE;
  }
  if (variable->value != NULL) {
    astTreeDelete(variable->value);
  }
  variable->value = value;
}

bool runAstTree(AstTreeRoot *root) {
  constexpr char MAIN_STR[] = "main";
  constexpr size_t MAIN_STR_SIZE =
      (sizeof(MAIN_STR) / sizeof(*MAIN_STR)) - sizeof(*MAIN_STR);

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    AstTree *variable_value = variable->value;
    size_t name_size = variable->name_end - variable->name_begin;
    if (name_size == MAIN_STR_SIZE &&
        strncmp(variable->name_begin, MAIN_STR, MAIN_STR_SIZE) == 0 &&
        variable_value->token == AST_TREE_TOKEN_FUNCTION) {

      AstTree *main = copyAstTree(variable_value);

      const bool ret =
          runAstTreeFunction(main, NULL, 0) == &AST_TREE_VOID_VALUE;
      astTreeDelete(main);
      return ret;
    }
  }
  printLog("main function is not found");
  return false;
}

AstTree *runAstTreeFunction(AstTree *tree, AstTreeFunctionCallParam *arguments,
                            size_t arguments_size) {
  AstTreeFunction *function = tree->metadata;

  bool shouldRet = false;

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTreeFunctionCallParam param = arguments[i];
    AstTreeVariable *arg = function->arguments.data[i];
    AstTree *value = runExpression(param.value, &shouldRet);
    runnerVariableSetValue(arg, value);
  }

  shouldRet = false;

  AstTree *ret = &AST_TREE_VOID_VALUE;

  for (size_t i = 0; i < function->scope.expressions_size; ++i) {
    astTreeDelete(ret);
    ret = runExpression(function->scope.expressions[i], &shouldRet);
    if (shouldRet) {
      break;
    }
  }

  return ret;
}

AstTree *runExpression(AstTree *expr, bool *shouldRet) {
  switch (expr->token) {
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTree *tree = runExpression(metadata, shouldRet);
    printf("%lu", *(AstTreeInt *)tree->metadata);
    astTreeDelete(tree);
    return &AST_TREE_VOID_VALUE;
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = expr->metadata;
    if (metadata->function->token == AST_TREE_TOKEN_VARIABLE) {
      AstTreeVariable *variable = metadata->function->metadata;
      AstTree *function = copyAstTree(variable->value);
      AstTree *result = runAstTreeFunction(function, metadata->parameters,
                                           metadata->parameters_size);
      astTreeDelete(function);
      return result;
    } else {
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreeInfix *metadata = expr->metadata;
    AstTreeVariable *left;
    if (metadata->left.token == AST_TREE_TOKEN_VARIABLE) {
      left = metadata->left.metadata;
    } else {
      AstTree *left_metadata = metadata->left.metadata;
      AstTree *var = runExpression(left_metadata, shouldRet);
      if (var->token != AST_TREE_TOKEN_VARIABLE) {
        UNREACHABLE;
      }
      left = var->metadata;
      astTreeDelete(var);
    }
    runnerVariableSetValue(left, runExpression(&metadata->right, shouldRet));
    return copyAstTree(left->value);
  }
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = expr->metadata;
    *shouldRet = true;
    if (metadata->value != NULL) {
      return runExpression(metadata->value, shouldRet);
    } else {
      return &AST_TREE_VOID_VALUE;
    }
  }
  case AST_TREE_TOKEN_VARIABLE_DEFINE: {
    AstTreeVariable *variable = expr->metadata;
    runnerVariableSetValue(variable, runExpression(variable->value, shouldRet));
    return &AST_TREE_VOID_VALUE;
  }
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = expr->metadata;
    AstTree *condition = runExpression(metadata->condition, shouldRet);
    AstTree *ret;
    if (*(AstTreeBool *)condition->metadata) {
      ret = runExpression(metadata->ifBody, shouldRet);
    } else if (metadata->elseBody != NULL) {
      ret = runExpression(metadata->elseBody, shouldRet);
    } else {
      ret = &AST_TREE_VOID_VALUE;
    }
    astTreeDelete(condition);
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = expr->metadata;
    AstTree *ret = NULL;
    while (!*shouldRet) {
      AstTree *tree = runExpression(metadata->condition, shouldRet);
      bool conti = *(AstTreeBool *)tree->metadata;
      astTreeDelete(tree);
      if (!conti) {
        break;
      }
      ret = runExpression(metadata->body, shouldRet);
    }
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *operand = expr->metadata;
    return runExpression((AstTree *)operand, shouldRet);
  }
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = expr->metadata;

    AstTree *ret = &AST_TREE_VOID_VALUE;
    for (size_t i = 0; i < metadata->expressions_size && !*shouldRet; ++i) {
      astTreeDelete(ret);
      ret = runExpression(metadata->expressions[i], shouldRet);
    }
    return ret;
  }
  case AST_TREE_TOKEN_OPERATOR_PLUS: {
    AstTreeSingleChild *operand = runExpression(expr->metadata, shouldRet);
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
      doLeftOperation(operand, +, AstTreeFloat, f128);
    } else if (operand->type == &AST_TREE_F64_TYPE) {
      doLeftOperation(operand, +, AstTreeFloat, f64);
    } else if (operand->type == &AST_TREE_F32_TYPE) {
      doLeftOperation(operand, +, AstTreeFloat, f32);
    } else if (operand->type == &AST_TREE_F16_TYPE) {
      doLeftOperation(operand, +, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeSingleChild *operand = runExpression(expr->metadata, shouldRet);
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
      doLeftOperation(operand, -, AstTreeFloat, f128);
    } else if (operand->type == &AST_TREE_F64_TYPE) {
      doLeftOperation(operand, -, AstTreeFloat, f64);
    } else if (operand->type == &AST_TREE_F32_TYPE) {
      doLeftOperation(operand, -, AstTreeFloat, f32);
    } else if (operand->type == &AST_TREE_F16_TYPE) {
      doLeftOperation(operand, -, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_SUM: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

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
      doOperation(left, right, +, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperation(left, right, +, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperation(left, right, +, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, +, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SUB: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

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
      doOperation(left, right, -, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperation(left, right, -, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperation(left, right, -, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, -, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

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
      doOperation(left, right, *, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperation(left, right, *, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperation(left, right, *, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, *, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_DIVIDE: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

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
      doOperation(left, right, /, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doOperation(left, right, /, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doOperation(left, right, /, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, /, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_MODULO: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

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
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeFloat, f16);
    } else if (left->type == &AST_TREE_TYPE_TYPE) {
      bool *res = a404m_malloc(sizeof(*res));
      *res = typeIsEqual(left, right);
      astTreeDelete(left);
      left = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, res, &AST_TREE_BOOL_TYPE,
                        NULL, NULL);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeFloat, f16);
    } else if (left->type == &AST_TREE_TYPE_TYPE) {
      bool *res = a404m_malloc(sizeof(*res));
      *res = !typeIsEqual(left, right);
      astTreeDelete(left);
      left = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, res, &AST_TREE_BOOL_TYPE,
                        NULL, NULL);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_GREATER: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doLogicalOperation(left, right, >, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doLogicalOperation(left, right, >, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doLogicalOperation(left, right, >, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doLogicalOperation(left, right, >, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, >, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SMALLER: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doLogicalOperation(left, right, <, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doLogicalOperation(left, right, <, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doLogicalOperation(left, right, <, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doLogicalOperation(left, right, <, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, <, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeFloat, f16);
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(&metadata->left, shouldRet);
    AstTree *right = runExpression(&metadata->right, shouldRet);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, u64);
    } else if (left->type == &AST_TREE_I64_TYPE &&
               right->type == &AST_TREE_I64_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, i64);
    } else if (left->type == &AST_TREE_U32_TYPE &&
               right->type == &AST_TREE_U32_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, u32);
    } else if (left->type == &AST_TREE_I32_TYPE &&
               right->type == &AST_TREE_I32_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, i32);
    } else if (left->type == &AST_TREE_U16_TYPE &&
               right->type == &AST_TREE_U16_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, u16);
    } else if (left->type == &AST_TREE_I16_TYPE &&
               right->type == &AST_TREE_I16_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, i16);
    } else if (left->type == &AST_TREE_U8_TYPE &&
               right->type == &AST_TREE_U8_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, u8);
    } else if (left->type == &AST_TREE_I8_TYPE &&
               right->type == &AST_TREE_I8_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeInt, i8);
    } else if (left->type == &AST_TREE_F128_TYPE &&
               right->type == &AST_TREE_F128_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeFloat, f128);
    } else if (left->type == &AST_TREE_F64_TYPE &&
               right->type == &AST_TREE_F64_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeFloat, f64);
    } else if (left->type == &AST_TREE_F32_TYPE &&
               right->type == &AST_TREE_F32_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeFloat, f32);
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeFloat, f16);
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
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_FUNCTION:
    return copyAstTree(expr);
  case AST_TREE_TOKEN_OPERATOR_ADDRESS: {
    AstTreeSingleChild *metadata = expr->metadata;
    if (metadata->token != AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
    }
    return copyAstTree(metadata);
  }
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTree *operand = runExpression(metadata, shouldRet);
    if (operand->token != AST_TREE_TOKEN_VARIABLE) {
      printLog("%s", AST_TREE_TOKEN_STRINGS[operand->token]);
      UNREACHABLE;
    }
    AstTreeVariable *variable = operand->metadata;
    AstTree *ret = copyAstTree(variable->value);
    astTreeDelete(operand);
    return ret;
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = expr->metadata;
    return copyAstTree(variable->value);
  }
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}
