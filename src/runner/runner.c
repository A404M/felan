#include "runner.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
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
  runnerVariableSetValueWihtoutConstCheck(variable, value);
}

void runnerVariableSetValueWihtoutConstCheck(AstTreeVariable *variable,
                                             AstTree *value) {
  if (variable->value != NULL) {
    astTreeDelete(variable->value);
  }
  variable->value = value;
}

bool runAstTree(AstTreeRoots roots) {
  static const char MAIN_STR[] = "main";
  static const size_t MAIN_STR_SIZE =
      (sizeof(MAIN_STR) / sizeof(*MAIN_STR)) - sizeof(*MAIN_STR);

  AstTreeVariable *mainVariable = NULL;

  for (size_t i = 0; i < roots.size; ++i) {
    AstTreeRoot *root = roots.data[i];
    for (size_t i = 0; i < root->variables.size; ++i) {
      AstTreeVariable *variable = root->variables.data[i];
      size_t name_size = variable->name_end - variable->name_begin;
      if (name_size == MAIN_STR_SIZE &&
          strncmp(variable->name_begin, MAIN_STR, MAIN_STR_SIZE) == 0 &&
          variable->type->token == AST_TREE_TOKEN_TYPE_FUNCTION) {
        if (mainVariable != NULL) {
          printLog("Too many main variables");
          return false;
        }
        mainVariable = variable;
      }
      if (!variable->isConst) {
        runnerVariableSetValueWihtoutConstCheck(variable, variable->initValue);
      }
    }
  }

  if (mainVariable == NULL) {
    printLog("main function is not found");
    return false;
  }

  AstTree *main;
  if (mainVariable->value != NULL) {
    main = copyAstTree(mainVariable->value);
  } else {
    printLog("main has no value");
    return false;
  }

  AstTree *res = runAstTreeFunction(main, NULL, 0);
  const bool ret = res == &AST_TREE_VOID_VALUE;
  astTreeDelete(res);
  astTreeDelete(main);
  return ret;
}

AstTree *runAstTreeFunction(AstTree *tree, AstTreeFunctionCallParam *arguments,
                            size_t arguments_size) {
  AstTreeFunction *function = tree->metadata;

  bool shouldRet = false;

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTreeFunctionCallParam param = arguments[i];
    AstTreeVariable *arg = function->arguments.data[i];
    AstTree *value =
        runExpression(param.value, &function->scope, &shouldRet, false);
    runnerVariableSetValueWihtoutConstCheck(arg, value);
  }

  shouldRet = false;

  for (size_t i = 0; i < function->scope.expressions_size; ++i) {
    AstTree *ret = runExpression(function->scope.expressions[i],
                                 &function->scope, &shouldRet, false);
    if (shouldRet) {
      return ret;
    } else {
      astTreeDelete(ret);
    }
  }

  return &AST_TREE_VOID_VALUE;
}

AstTree *runAstTreeBuiltin(AstTree *tree, AstTreeScope *scope,
                           AstTreeFunctionCallParam *arguments,
                           size_t arguments_size) {
  AstTreeBuiltin *metadata = tree->metadata;

  AstTrees args = {
      .data = a404m_malloc(arguments_size * sizeof(*args.data)),
      .size = arguments_size,
  };

  bool shouldRet = false;

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTreeFunctionCallParam param = arguments[i];
    args.data[i] = runExpression(param.value, scope, &shouldRet, false);
  }

  if (shouldRet) {
    UNREACHABLE;
  }

  AstTree *ret;

  switch (metadata->token) {
  case AST_TREE_BUILTIN_TOKEN_CAST: {
    AstTree *from = args.data[0];
    AstTree *to = args.data[1];

    if (from->token == AST_TREE_TOKEN_VALUE_INT) {
      AstTreeInt value = *(AstTreeInt *)from->metadata;
      if (typeIsEqual(to, &AST_TREE_U8_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u8)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U8_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U16_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U16_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U32_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U32_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U64_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U64_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I8_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i8)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I8_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I16_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I16_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I32_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I32_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I64_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I64_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F16_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F16_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F32_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F32_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F64_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F64_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F128_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f128)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F128_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_BOOL_TYPE)) {
        AstTreeBool *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (bool)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, newValue,
                         &AST_TREE_BOOL_TYPE, NULL, NULL);
      } else {
        printError(to->str_begin, to->str_end, "Bad type %s",
                   AST_TREE_TOKEN_STRINGS[to->token]);
        UNREACHABLE;
      }
    } else if (from->token == AST_TREE_TOKEN_VALUE_FLOAT) {
      AstTreeFloat value = *(AstTreeFloat *)from->metadata;
      if (typeIsEqual(to, &AST_TREE_U8_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u8)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U8_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U16_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U16_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U32_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U32_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U64_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U64_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I8_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i8)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I8_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I16_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I16_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I32_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I32_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I64_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I64_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F16_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F16_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F32_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F32_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F64_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F64_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F128_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f128)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F128_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_BOOL_TYPE)) {
        AstTreeBool *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (bool)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, newValue,
                         &AST_TREE_BOOL_TYPE, NULL, NULL);
      } else {
        printError(to->str_begin, to->str_end, "Bad type %s",
                   AST_TREE_TOKEN_STRINGS[to->token]);
        UNREACHABLE;
      }
    } else if (from->token == AST_TREE_TOKEN_VALUE_BOOL) {
      AstTreeBool value = *(AstTreeBool *)from->metadata;
      if (typeIsEqual(to, &AST_TREE_U8_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u8)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U8_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U16_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U16_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U32_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U32_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_U64_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (u64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_U64_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I8_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i8)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I8_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I16_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I16_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I32_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I32_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_I64_TYPE)) {
        AstTreeInt *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (i64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_INT, newValue, &AST_TREE_I64_TYPE,
                         NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F16_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f16)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F16_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F32_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f32)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F32_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F64_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f64)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F64_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_F128_TYPE)) {
        AstTreeFloat *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (f128)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_FLOAT, newValue,
                         &AST_TREE_F128_TYPE, NULL, NULL);
      } else if (typeIsEqual(to, &AST_TREE_BOOL_TYPE)) {
        AstTreeBool *newValue = a404m_malloc(sizeof(*newValue));
        *newValue = (bool)value;
        ret = newAstTree(AST_TREE_TOKEN_VALUE_BOOL, newValue,
                         &AST_TREE_BOOL_TYPE, NULL, NULL);
      } else {
        printError(to->str_begin, to->str_end, "Bad type %s",
                   AST_TREE_TOKEN_STRINGS[to->token]);
        UNREACHABLE;
      }
    } else {
      printError(to->str_begin, to->str_end, "Bad token %s",
                 AST_TREE_TOKEN_STRINGS[from->token]);
      UNREACHABLE;
    }
    goto RETURN;
  }
  case AST_TREE_BUILTIN_TOKEN_TYPE_OF: {
    AstTree *variable = args.data[0];
    ret = copyAstTree(variable->type);
  }
    goto RETURN;
  case AST_TREE_BUILTIN_TOKEN_IMPORT:
  case AST_TREE_BUILTIN_TOKEN__SIZE__:
  }
  UNREACHABLE;

RETURN:
  for (size_t i = 0; i < args.size; ++i) {
    astTreeDelete(args.data[i]);
  }
  free(args.data);

  return ret;
}

AstTree *runExpression(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                       bool isLeft) {
  switch (expr->token) {
  case AST_TREE_TOKEN_KEYWORD_PUTC: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTree *tree = runExpression(metadata, scope, shouldRet, false);
    putchar((u8) * (AstTreeInt *)tree->metadata);
    astTreeDelete(tree);
    return &AST_TREE_VOID_VALUE;
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = expr->metadata;
    AstTree *function =
        runExpression(metadata->function, scope, shouldRet, false);
    AstTree *result;
    if (function->token == AST_TREE_TOKEN_FUNCTION) {
      result = runAstTreeFunction(function, metadata->parameters,
                                  metadata->parameters_size);
    } else if (function->token == AST_TREE_TOKEN_BUILTIN) {
      result = runAstTreeBuiltin(function, scope, metadata->parameters,
                                 metadata->parameters_size);
    } else {
      UNREACHABLE;
    }
    astTreeDelete(function);
    return result;
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *l = runExpression(metadata->left, scope, shouldRet, true);
    if (l->token != AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
    }
    AstTreeVariable *left = l->metadata;
    runnerVariableSetValue(
        left, runExpression(metadata->right, scope, shouldRet, false));
    astTreeDelete(l);
    return copyAstTree(left->value);
  }
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = expr->metadata;
    *shouldRet = true;
    if (metadata->value != NULL) {
      return runExpression(metadata->value, scope, shouldRet, false);
    } else {
      return &AST_TREE_VOID_VALUE;
    }
  }
  case AST_TREE_TOKEN_VARIABLE_DEFINE: {
    AstTreeVariable *variable = expr->metadata;
    runnerVariableSetValue(
        variable, runExpression(variable->initValue, scope, shouldRet, false));
    return &AST_TREE_VOID_VALUE;
  }
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = expr->metadata;
    AstTree *condition =
        runExpression(metadata->condition, scope, shouldRet, false);
    AstTree *ret;
    if (*(AstTreeBool *)condition->metadata) {
      ret = runExpression(metadata->ifBody, scope, shouldRet, isLeft);
    } else if (metadata->elseBody != NULL) {
      ret = runExpression(metadata->elseBody, scope, shouldRet, isLeft);
    } else {
      ret = &AST_TREE_VOID_VALUE;
    }
    astTreeDelete(condition);
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = expr->metadata;
    AstTree *ret = &AST_TREE_VOID_VALUE;
    while (!*shouldRet) {
      AstTree *tree =
          runExpression(metadata->condition, scope, shouldRet, false);
      bool conti = *(AstTreeBool *)tree->metadata;
      astTreeDelete(tree);
      if (!conti) {
        break;
      }
      astTreeDelete(ret);
      ret = runExpression(metadata->body, scope, shouldRet, isLeft);
    }
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *operand = expr->metadata;
    return runExpression((AstTree *)operand, scope, shouldRet, isLeft);
  }
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = expr->metadata;

    AstTree *ret = &AST_TREE_VOID_VALUE;
    for (size_t i = 0; i < metadata->expressions_size && !*shouldRet; ++i) {
      astTreeDelete(ret);
      ret = runExpression(metadata->expressions[i], scope, shouldRet,
                          i == metadata->expressions_size - 1 && isLeft);
    }
    return ret;
  }
  case AST_TREE_TOKEN_OPERATOR_PLUS: {
    AstTreeSingleChild *operand =
        runExpression(expr->metadata, scope, shouldRet, false);
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
#ifdef FLOAT_16_SUPPORT
    } else if (operand->type == &AST_TREE_F16_TYPE) {
      doLeftOperation(operand, +, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeSingleChild *operand =
        runExpression(expr->metadata, scope, shouldRet, false);
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
#ifdef FLOAT_16_SUPPORT
    } else if (operand->type == &AST_TREE_F16_TYPE) {
      doLeftOperation(operand, -, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT: {
    AstTreeSingleChild *operand =
        runExpression(expr->metadata, scope, shouldRet, false);

    *(AstTreeBool *)operand->metadata = !*((AstTreeBool *)operand->metadata);
    return operand;
  }
  case AST_TREE_TOKEN_OPERATOR_SUM: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, +, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SUB: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, -, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, *, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_DIVIDE: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doOperation(left, right, /, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_MODULO: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
      printError(expr->str_begin, expr->str_end, "Not supported %s",
                 AST_TREE_TOKEN_STRINGS[right->type->token]);
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, ==, AstTreeFloat, f16);
#endif
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
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, !=, AstTreeFloat, f16);
#endif
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
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, >, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SMALLER: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, <, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, >=, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

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
#ifdef FLOAT_16_SUPPORT
    } else if (left->type == &AST_TREE_F16_TYPE &&
               right->type == &AST_TREE_F16_TYPE) {
      doLogicalOperation(left, right, <=, AstTreeFloat, f16);
#endif
    } else {
      printError(expr->str_begin, expr->str_end, "Not supported");
      UNREACHABLE;
    }
    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

    *(AstTreeBool *)left->metadata =
        *(AstTreeBool *)left->metadata && *(AstTreeBool *)right->metadata;

    astTreeDelete(right);
    return left;
  }
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR: {
    AstTreeInfix *metadata = expr->metadata;
    AstTree *left = runExpression(metadata->left, scope, shouldRet, false);
    AstTree *right = runExpression(metadata->right, scope, shouldRet, false);

    *(AstTreeBool *)left->metadata =
        *(AstTreeBool *)left->metadata || *(AstTreeBool *)right->metadata;

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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN:
  case AST_TREE_TOKEN_TYPE_ARRAY:
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
    AstTree *operand = runExpression(metadata, scope, shouldRet, false);
    if (operand->token != AST_TREE_TOKEN_VARIABLE) {
      printLog("%s", AST_TREE_TOKEN_STRINGS[operand->token]);
      UNREACHABLE;
    }
    AstTree *ret;
    if (isLeft) {
      ret = operand;
    } else {
      AstTreeVariable *variable = operand->metadata;
      ret = copyAstTree(variable->value);
      astTreeDelete(operand);
    }
    return ret;
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = expr->metadata;
    if (isLeft) {
      return copyAstTree(expr);
    } else {
      if (variable->value == NULL) {
        UNREACHABLE;
      }
      return copyAstTree(variable->value);
    }
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = expr->metadata;
    AstTree *tree = runExpression(metadata->object, scope, shouldRet, true);
    if (tree->token != AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
    }
    AstTreeVariable *variable = tree->metadata;
    astTreeDelete(tree);
    if (variable->type->token == AST_TREE_TOKEN_TYPE_ARRAY) {
      AstTreeBracket *array_metadata = variable->type->metadata;
      if (metadata->member.index != 0) {
        UNREACHABLE;
      } else if (variable->value->token == AST_TREE_TOKEN_VALUE_UNDEFINED) {
        if (array_metadata->parameters.size == 0) {
          UNREACHABLE;
        } else {
          AstTree *sizeTree = runExpression(array_metadata->parameters.data[0],
                                            scope, shouldRet, false);
          if (sizeTree->token != AST_TREE_TOKEN_VALUE_INT) {
            UNREACHABLE;
          } else {
            return sizeTree;
          }
        }
      } else if (variable->value->token == AST_TREE_TOKEN_VALUE_OBJECT) {
        AstTreeObject *object = variable->value->metadata;
        AstTreeInt *res_metadata = a404m_malloc(sizeof(*res_metadata));
        *res_metadata = object->variables.size;
        return newAstTree(AST_TREE_TOKEN_VALUE_INT, res_metadata,
                          &AST_TREE_U64_TYPE, NULL, NULL);
      }
    } else if (variable->type->token == AST_TREE_TOKEN_KEYWORD_STRUCT) {
      if (variable->value->token == AST_TREE_TOKEN_VALUE_UNDEFINED) {
        AstTreeStruct *struc = variable->type->metadata;
        AstTreeObject *newMetadata = a404m_malloc(sizeof(*newMetadata));

        newMetadata->variables =
            copyAstTreeVariables(struc->variables, NULL, NULL, 0);

        for (size_t i = 0; i < newMetadata->variables.size; ++i) {
          AstTreeVariable *member = newMetadata->variables.data[i];
          if (!member->isConst) {
            runnerVariableSetValue(member,
                                   newAstTree(AST_TREE_TOKEN_VALUE_UNDEFINED,
                                              NULL, copyAstTree(member->type),
                                              variable->value->str_begin,
                                              variable->value->str_end));
          }
        }

        runnerVariableSetValue(variable, newAstTree(AST_TREE_TOKEN_VALUE_OBJECT,
                                                    newMetadata,
                                                    copyAstTree(variable->type),
                                                    variable->value->str_begin,
                                                    variable->value->str_end));
      }
      AstTreeObject *object = variable->value->metadata;
      AstTreeVariable *var = object->variables.data[metadata->member.index];
      if (isLeft) {
        return newAstTree(AST_TREE_TOKEN_VARIABLE, var, copyAstTree(var->type),
                          var->name_begin, var->name_end);
      } else {
        return copyAstTree(var->value);
      }
    }
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    expr = copyAstTree(expr);
    AstTreeStruct *metadata = expr->metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *member = metadata->variables.data[i];
      AstTree *type = member->type;
      member->type = runExpression(member->type, scope, shouldRet, isLeft);
      if (type != member->type) {
        astTreeDelete(type);
      }
    }
    return expr;
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTreeSingleChild *newMetadata =
        runExpression(metadata, scope, shouldRet, isLeft);

    return newAstTree(AST_TREE_TOKEN_OPERATOR_POINTER, newMetadata,
                      copyAstTree(expr->type), expr->str_begin, expr->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = expr->metadata;
    AstTree *operand = runExpression(metadata->operand, scope, shouldRet, true);

    if (operand->token != AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
    } else if (metadata->parameters.size != 1) {
      UNREACHABLE;
    }

    AstTree *array_indexNode =
        runExpression(metadata->parameters.data[0], scope, shouldRet, false);

    if (array_indexNode->token != AST_TREE_TOKEN_VALUE_INT) {
      UNREACHABLE;
    }

    AstTreeInt index = *(AstTreeInt *)array_indexNode->metadata;
    astTreeDelete(array_indexNode);

    AstTreeVariable *variable = operand->metadata;
    astTreeDelete(operand);
    if (variable->value->token == AST_TREE_TOKEN_VALUE_UNDEFINED) {
      AstTreeBracket *array_type_metadata = variable->type->metadata;
      if (array_type_metadata->parameters.size != 1) {
        UNREACHABLE;
      }
      AstTree *arraySize_tree = runExpression(
          array_type_metadata->parameters.data[0], scope, shouldRet, false);
      if (arraySize_tree->token != AST_TREE_TOKEN_VALUE_INT) {
        UNREACHABLE;
      }
      AstTreeInt array_size = *(AstTreeInt *)arraySize_tree->metadata;
      astTreeDelete(arraySize_tree);

      AstTreeObject *newMetadata = a404m_malloc(sizeof(*newMetadata));

      newMetadata->variables = (AstTreeVariables){
          .data =
              a404m_malloc(array_size * sizeof(*newMetadata->variables.data)),
          .size = array_size,
      };

      for (size_t i = 0; i < array_size; ++i) {
        AstTreeVariable *member = a404m_malloc(sizeof(*member));
        member->name_begin = member->name_end = NULL;
        member->isConst = false;
        member->type = copyAstTree(array_type_metadata->operand);
        member->value = newAstTree(
            AST_TREE_TOKEN_VALUE_UNDEFINED, NULL, copyAstTree(member->type),
            variable->value->str_begin, variable->value->str_end);
        member->initValue = NULL;
        newMetadata->variables.data[i] = member;
      }

      runnerVariableSetValue(variable, newAstTree(AST_TREE_TOKEN_VALUE_OBJECT,
                                                  newMetadata,
                                                  copyAstTree(variable->type),
                                                  variable->value->str_begin,
                                                  variable->value->str_end));
    }
    AstTreeObject *object = variable->value->metadata;
    AstTreeVariable *var = object->variables.data[index];

    if (isLeft) {
      return newAstTree(AST_TREE_TOKEN_VARIABLE, var, copyAstTree(var->type),
                        var->name_begin, var->name_end);
    } else {
      return copyAstTree(var->value);
    }
  }
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}
