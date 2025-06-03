#include "runner.h"
#include "compiler/ast-tree.h"
#include "utils/dl.h"
#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"
#include "utils/type.h"
#include <dlfcn.h>
#include <ffi.h>
#include <iso646.h>
#include <stdatomic.h>
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
    (op0)->token = AST_TREE_TOKEN_RAW_VALUE;                                   \
  }

#define doLeftOperation(op0, operator, originalType, type)                     \
  *((originalType *)(op0)->metadata) = operator(                               \
      (type) * (originalType *)(op0)->metadata)

#define doCast(left, type, to)                                                 \
  {                                                                            \
    type *newValue = a404m_malloc(sizeof(*newValue));                          \
    *newValue = left;                                                          \
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, newValue, copyAstTree(to),     \
                      NULL, NULL);                                             \
  }

#ifdef FLOAT_16_SUPPORT
#define doCastAll(left, left_type, to)                                         \
  {                                                                            \
    const left_type value = *(left_type *)left->metadata;                      \
    switch (to->token) {                                                       \
    case AST_TREE_TOKEN_TYPE_I8:                                               \
      doCast(value, i8, to);                                                   \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_U8:                                               \
      doCast(value, u8, to);                                                   \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_I16:                                              \
      doCast(value, i16, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_U16:                                              \
      doCast(value, u16, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_I32:                                              \
      doCast(value, i32, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_U32:                                              \
      doCast(value, u32, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_I64:                                              \
      doCast(value, i64, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_U64:                                              \
      doCast(value, u64, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_F16:                                              \
      doCast(value, f16, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_F32:                                              \
      doCast(value, f32, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_F64:                                              \
      doCast(value, f64, to);                                                  \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_F128:                                             \
      doCast(value, f128, to);                                                 \
      break;                                                                   \
    case AST_TREE_TOKEN_TYPE_BOOL:                                             \
      doCast(value, bool, to);                                                 \
      break;                                                                   \
    default:                                                                   \
      UNREACHABLE;                                                             \
    }                                                                          \
  }
#else
#define doCastAll(left, left_type, type, to)                                   \
  {                                                                            \
    const left_type value = *(left_type *)left->metadata;                      \
    switch (to->token) {                                                       \
    case AST_TREE_TOKEN_TYPE_I8:                                               \
      doCast(value, i8, to);                                                   \
    case AST_TREE_TOKEN_TYPE_U8:                                               \
      doCast(value, u8, to);                                                   \
    case AST_TREE_TOKEN_TYPE_I16:                                              \
      doCast(value, i16, to);                                                  \
    case AST_TREE_TOKEN_TYPE_U16:                                              \
      doCast(value, u16, to);                                                  \
    case AST_TREE_TOKEN_TYPE_I32:                                              \
      doCast(value, i32, to);                                                  \
    case AST_TREE_TOKEN_TYPE_U32:                                              \
      doCast(value, u32, to);                                                  \
    case AST_TREE_TOKEN_TYPE_I64:                                              \
      doCast(value, i64, to);                                                  \
    case AST_TREE_TOKEN_TYPE_U64:                                              \
      doCast(value, u64, to);                                                  \
    case AST_TREE_TOKEN_TYPE_F32:                                              \
      doCast(value, f32, to);                                                  \
    case AST_TREE_TOKEN_TYPE_F64:                                              \
      doCast(value, f64, to);                                                  \
    case AST_TREE_TOKEN_TYPE_F128:                                             \
      doCast(value, f128, to);                                                 \
    case AST_TREE_TOKEN_TYPE_BOOL:                                             \
      doCast(value, bool, to);                                                 \
    default:                                                                   \
      UNREACHABLE;                                                             \
    }                                                                          \
  }
#endif

void runnerVariableSetValue(AstTreeVariable *variable, AstTree *value,
                            AstTreeScope *scope) {
  if (variable->isConst) {
    printLog("Can't assign to const");
    UNREACHABLE;
  }
  runnerVariableSetValueWihtoutConstCheck(variable, value, scope);
}

void runnerVariableSetValueWihtoutConstCheck(AstTreeVariable *variable,
                                             AstTree *value,
                                             AstTreeScope *scope) {
  if (variable->value != NULL) {
    astTreeDelete(variable->value);
  }
  AstTree *raw = toRawValue(value, scope);
  if (raw == NULL) {
    variable->value = value;
  } else {
    variable->value = raw;
    astTreeDelete(value);
  }
}

bool runAstTree(AstTreeRoots roots) {
  static const char MAIN_STR[] = "main";
  static const size_t MAIN_STR_SIZE =
      (sizeof(MAIN_STR) / sizeof(*MAIN_STR)) - sizeof(*MAIN_STR);

  AstTreeVariable *mainVariable = NULL;

  AstTreeScope *scope = a404m_malloc(sizeof(*scope));
  *scope = (AstTreeScope){
      .expressions = a404m_malloc(0),
      .expressions_size = 0,
      .variables.data = a404m_malloc(0),
      .variables.size = 0,
      .stackAllocation = a404m_malloc(0),
      .stackAllocation_size = 0,
  };

  for (size_t i = 0; i < roots.size; ++i) {
    AstTreeRoot *root = roots.data[i];
    for (size_t i = 0; i < root->variables.size; ++i) {
      AstTreeVariable *variable = root->variables.data[i];
      size_t name_size = variable->name_end - variable->name_begin;
      if (name_size == MAIN_STR_SIZE &&
          strnEquals(variable->name_begin, MAIN_STR, MAIN_STR_SIZE) &&
          variable->type->token == AST_TREE_TOKEN_TYPE_FUNCTION) {
        if (mainVariable != NULL) {
          printLog("Too many main variables");
          return false;
        }
        mainVariable = variable;
      }
      if (!variable->isConst) {
        runnerVariableSetValueWihtoutConstCheck(variable, variable->initValue,
                                                scope);
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

  AstTree *res = runAstTreeFunction(main, NULL, 0, scope, false);
  const bool ret = res == &AST_TREE_VOID_VALUE;
  astTreeDelete(res);
  astTreeDelete(main);
  astTreeDelete(
      newAstTree(AST_TREE_TOKEN_SCOPE, scope, &AST_TREE_VOID_TYPE, NULL, NULL));
  return ret;
}

AstTree *runAstTreeFunction(AstTree *tree, AstTree **arguments,
                            size_t arguments_size, AstTreeScope *scope,
                            bool isComptime) {
  AstTreeFunction *function = tree->metadata;

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTree *param = arguments[i];
    AstTreeVariable *arg = function->arguments.data[i];
    runnerVariableSetValueWihtoutConstCheck(arg, param, scope);
  }

  bool shouldRet = false;
  u32 breakCount = 0;
  bool shouldContinue = false;

  for (size_t i = 0; i < function->scope.expressions_size; ++i) {
    AstTree *ret = runExpression(
        function->scope.expressions[i], &function->scope, &shouldRet, false,
        isComptime, &breakCount, &shouldContinue, false);
    if (shouldRet) {
      return ret;
    } else {
      astTreeDelete(ret);
    }
  }

  return &AST_TREE_VOID_VALUE;
}

AstTree *runAstTreeBuiltin(AstTree *tree, AstTreeScope *scope,
                           AstTree **arguments) {
  (void)scope;
  switch (tree->token) {
  case AST_TREE_TOKEN_BUILTIN_CAST: {
    AstTree *from = arguments[0];
    AstTree *to = arguments[1];

    return castTo(from, to);
  }
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF: {
    AstTree *variable = arguments[0];
    return copyAstTree(variable->type);
  }
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF: {
    AstTree *type = arguments[0];
    AstTreeRawValue *value = a404m_malloc(getSizeOfType(&AST_TREE_U64_TYPE));
    *(u64 *)value = getSizeOfType(type);
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value, &AST_TREE_U64_TYPE, NULL,
                      NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_NEG: {
    AstTree *left = arguments[0];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));
    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = -*(i8 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = -*(u8 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = -*(i16 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = -*(u16 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = -*(i32 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = -*(u32 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = -*(i64 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = -*(u64 *)left->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(f16 *)ret = -*(f16 *)left->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(f32 *)ret = -*(f32 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(f64 *)ret = -*(f64 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(f128 *)ret = -*(f128 *)left->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_ADD: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata + *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata + *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata + *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata + *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata + *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata + *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata + *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata + *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(f16 *)ret = *(f16 *)left->metadata + *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(f32 *)ret = *(f32 *)left->metadata + *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(f64 *)ret = *(f64 *)left->metadata + *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(f128 *)ret = *(f128 *)left->metadata + *(f128 *)right->metadata;
      break;
    case AST_TREE_TOKEN_OPERATOR_POINTER:
      if (typeIsEqual(right->type, &AST_TREE_I64_TYPE)) {
        *(u8 **)ret =
            *(u8 **)left->metadata +
            *(i64 *)right->metadata *
                getSizeOfType((AstTreeSingleChild *)left->type->metadata);
        break;
      } else if (typeIsEqual(right->type, &AST_TREE_U64_TYPE)) {
        *(u8 **)ret =
            *(u8 **)left->metadata +
            *(u64 *)right->metadata *
                getSizeOfType((AstTreeSingleChild *)left->type->metadata);
        break;
      } else {
        printLog("%s", AST_TREE_TOKEN_STRINGS[right->type->token]);
        UNREACHABLE;
      }
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_SUB: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata - *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata - *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata - *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata - *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata - *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata - *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata - *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata - *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(f16 *)ret = *(f16 *)left->metadata - *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(f32 *)ret = *(f32 *)left->metadata - *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(f64 *)ret = *(f64 *)left->metadata - *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(f128 *)ret = *(f128 *)left->metadata - *(f128 *)right->metadata;
      break;
    case AST_TREE_TOKEN_OPERATOR_POINTER:
      if (typeIsEqual(right->type, &AST_TREE_I64_TYPE)) {
        *(u8 **)ret =
            *(u8 **)left->metadata -
            *(i64 *)right->metadata *
                getSizeOfType((AstTreeSingleChild *)left->type->metadata);
        break;
      } else if (typeIsEqual(right->type, &AST_TREE_U64_TYPE)) {
        *(u8 **)ret =
            *(u8 **)left->metadata -
            *(u64 *)right->metadata *
                getSizeOfType((AstTreeSingleChild *)left->type->metadata);
        break;
      } else {
        printLog("%s", AST_TREE_TOKEN_STRINGS[right->type->token]);
        UNREACHABLE;
      }
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_MUL: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata * *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata * *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata * *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata * *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata * *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata * *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata * *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata * *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(f16 *)ret = *(f16 *)left->metadata * *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(f32 *)ret = *(f32 *)left->metadata * *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(f64 *)ret = *(f64 *)left->metadata * *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(f128 *)ret = *(f128 *)left->metadata * *(f128 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_DIV: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata / *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata / *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata / *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata / *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata / *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata / *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata / *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata / *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(f16 *)ret = *(f16 *)left->metadata / *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(f32 *)ret = *(f32 *)left->metadata / *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(f64 *)ret = *(f64 *)left->metadata / *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(f128 *)ret = *(f128 *)left->metadata / *(f128 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_MOD: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata % *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata % *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata % *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata % *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata % *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata % *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata % *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata % *(u64 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_EQUAL: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];

    AstTree *ret =
        newAstTree(AST_TREE_TOKEN_RAW_VALUE, a404m_malloc(sizeof(AstTreeBool)),
                   &AST_TREE_BOOL_TYPE, NULL, NULL);

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(AstTreeBool *)ret->metadata =
          *(i8 *)left->metadata == *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(AstTreeBool *)ret->metadata =
          *(u8 *)left->metadata == *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(AstTreeBool *)ret->metadata =
          *(i16 *)left->metadata == *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(AstTreeBool *)ret->metadata =
          *(u16 *)left->metadata == *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(AstTreeBool *)ret->metadata =
          *(i32 *)left->metadata == *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(AstTreeBool *)ret->metadata =
          *(u32 *)left->metadata == *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(AstTreeBool *)ret->metadata =
          *(i64 *)left->metadata == *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(AstTreeBool *)ret->metadata =
          *(u64 *)left->metadata == *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(AstTreeBool *)ret->metadata =
          *(f16 *)left->metadata == *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(AstTreeBool *)ret->metadata =
          *(f32 *)left->metadata == *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(AstTreeBool *)ret->metadata =
          *(f64 *)left->metadata == *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(AstTreeBool *)ret->metadata =
          *(f128 *)left->metadata == *(f128 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_BOOL:
      *(AstTreeBool *)ret->metadata =
          *(AstTreeBool *)left->metadata == *(AstTreeBool *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_TYPE:
      *(AstTreeBool *)ret->metadata = typeIsEqual(left, right);
      break;
    default:
      printLog("%s", AST_TREE_TOKEN_STRINGS[left->token]);
      UNREACHABLE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];

    AstTree *ret =
        newAstTree(AST_TREE_TOKEN_RAW_VALUE, a404m_malloc(sizeof(AstTreeBool)),
                   &AST_TREE_BOOL_TYPE, NULL, NULL);

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(AstTreeBool *)ret->metadata =
          *(i8 *)left->metadata != *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(AstTreeBool *)ret->metadata =
          *(u8 *)left->metadata != *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(AstTreeBool *)ret->metadata =
          *(i16 *)left->metadata != *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(AstTreeBool *)ret->metadata =
          *(u16 *)left->metadata != *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(AstTreeBool *)ret->metadata =
          *(i32 *)left->metadata != *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(AstTreeBool *)ret->metadata =
          *(u32 *)left->metadata != *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(AstTreeBool *)ret->metadata =
          *(i64 *)left->metadata != *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(AstTreeBool *)ret->metadata =
          *(u64 *)left->metadata != *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(AstTreeBool *)ret->metadata =
          *(f16 *)left->metadata != *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(AstTreeBool *)ret->metadata =
          *(f32 *)left->metadata != *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(AstTreeBool *)ret->metadata =
          *(f64 *)left->metadata != *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(AstTreeBool *)ret->metadata =
          *(f128 *)left->metadata != *(f128 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_BOOL:
      *(AstTreeBool *)ret->metadata =
          *(AstTreeBool *)left->metadata == *(AstTreeBool *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_TYPE:
      *(AstTreeBool *)ret->metadata = !typeIsEqual(left, right);
      break;
    default:
      UNREACHABLE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_BUILTIN_GREATER: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];

    AstTree *ret =
        newAstTree(AST_TREE_TOKEN_RAW_VALUE, a404m_malloc(sizeof(AstTreeBool)),
                   &AST_TREE_BOOL_TYPE, NULL, NULL);

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(AstTreeBool *)ret->metadata =
          *(i8 *)left->metadata > *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(AstTreeBool *)ret->metadata =
          *(u8 *)left->metadata > *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(AstTreeBool *)ret->metadata =
          *(i16 *)left->metadata > *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(AstTreeBool *)ret->metadata =
          *(u16 *)left->metadata > *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(AstTreeBool *)ret->metadata =
          *(i32 *)left->metadata > *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(AstTreeBool *)ret->metadata =
          *(u32 *)left->metadata > *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(AstTreeBool *)ret->metadata =
          *(i64 *)left->metadata > *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(AstTreeBool *)ret->metadata =
          *(u64 *)left->metadata > *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(AstTreeBool *)ret->metadata =
          *(f16 *)left->metadata > *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(AstTreeBool *)ret->metadata =
          *(f32 *)left->metadata > *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(AstTreeBool *)ret->metadata =
          *(f64 *)left->metadata > *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(AstTreeBool *)ret->metadata =
          *(f128 *)left->metadata > *(f128 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_BUILTIN_SMALLER: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];

    AstTree *ret =
        newAstTree(AST_TREE_TOKEN_RAW_VALUE, a404m_malloc(sizeof(AstTreeBool)),
                   &AST_TREE_BOOL_TYPE, NULL, NULL);

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(AstTreeBool *)ret->metadata =
          *(i8 *)left->metadata < *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(AstTreeBool *)ret->metadata =
          *(u8 *)left->metadata < *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(AstTreeBool *)ret->metadata =
          *(i16 *)left->metadata < *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(AstTreeBool *)ret->metadata =
          *(u16 *)left->metadata < *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(AstTreeBool *)ret->metadata =
          *(i32 *)left->metadata < *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(AstTreeBool *)ret->metadata =
          *(u32 *)left->metadata < *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(AstTreeBool *)ret->metadata =
          *(i64 *)left->metadata < *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(AstTreeBool *)ret->metadata =
          *(u64 *)left->metadata < *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(AstTreeBool *)ret->metadata =
          *(f16 *)left->metadata < *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(AstTreeBool *)ret->metadata =
          *(f32 *)left->metadata < *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(AstTreeBool *)ret->metadata =
          *(f64 *)left->metadata < *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(AstTreeBool *)ret->metadata =
          *(f128 *)left->metadata < *(f128 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];

    AstTree *ret =
        newAstTree(AST_TREE_TOKEN_RAW_VALUE, a404m_malloc(sizeof(AstTreeBool)),
                   &AST_TREE_BOOL_TYPE, NULL, NULL);

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(AstTreeBool *)ret->metadata =
          *(i8 *)left->metadata >= *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(AstTreeBool *)ret->metadata =
          *(u8 *)left->metadata >= *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(AstTreeBool *)ret->metadata =
          *(i16 *)left->metadata >= *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(AstTreeBool *)ret->metadata =
          *(u16 *)left->metadata >= *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(AstTreeBool *)ret->metadata =
          *(i32 *)left->metadata >= *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(AstTreeBool *)ret->metadata =
          *(u32 *)left->metadata >= *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(AstTreeBool *)ret->metadata =
          *(i64 *)left->metadata >= *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(AstTreeBool *)ret->metadata =
          *(u64 *)left->metadata >= *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(AstTreeBool *)ret->metadata =
          *(f16 *)left->metadata >= *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(AstTreeBool *)ret->metadata =
          *(f32 *)left->metadata >= *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(AstTreeBool *)ret->metadata =
          *(f64 *)left->metadata >= *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(AstTreeBool *)ret->metadata =
          *(f128 *)left->metadata >= *(f128 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_BOOL:
      *(AstTreeBool *)ret->metadata =
          *(AstTreeBool *)left->metadata == *(AstTreeBool *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];

    AstTree *ret =
        newAstTree(AST_TREE_TOKEN_RAW_VALUE, a404m_malloc(sizeof(AstTreeBool)),
                   &AST_TREE_BOOL_TYPE, NULL, NULL);

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(AstTreeBool *)ret->metadata =
          *(i8 *)left->metadata <= *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(AstTreeBool *)ret->metadata =
          *(u8 *)left->metadata <= *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(AstTreeBool *)ret->metadata =
          *(i16 *)left->metadata <= *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(AstTreeBool *)ret->metadata =
          *(u16 *)left->metadata <= *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(AstTreeBool *)ret->metadata =
          *(i32 *)left->metadata <= *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(AstTreeBool *)ret->metadata =
          *(u32 *)left->metadata <= *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(AstTreeBool *)ret->metadata =
          *(i64 *)left->metadata <= *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(AstTreeBool *)ret->metadata =
          *(u64 *)left->metadata <= *(u64 *)right->metadata;
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      *(AstTreeBool *)ret->metadata =
          *(f16 *)left->metadata <= *(f16 *)right->metadata;
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      *(AstTreeBool *)ret->metadata =
          *(f32 *)left->metadata <= *(f32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(AstTreeBool *)ret->metadata =
          *(f64 *)left->metadata <= *(f64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(AstTreeBool *)ret->metadata =
          *(f128 *)left->metadata <= *(f128 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_BOOL:
      *(AstTreeBool *)ret->metadata =
          *(AstTreeBool *)left->metadata == *(AstTreeBool *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT: {
    AstTree *left = arguments[0];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata & *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata & *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata & *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata & *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata & *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata & *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata & *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata & *(u64 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata ^ *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata ^ *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata ^ *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata ^ *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata ^ *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata ^ *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata ^ *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata ^ *(u64 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata | *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata | *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata | *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata | *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata | *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata | *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata | *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata | *(u64 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata << *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata << *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata << *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata << *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata << *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata << *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata << *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata << *(u64 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT: {
    AstTree *left = arguments[0];
    AstTree *right = arguments[1];
    AstTreeRawValue *ret = a404m_malloc(getSizeOfType(left->type));

    switch (left->type->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      *(i8 *)ret = *(i8 *)left->metadata >> *(i8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      *(u8 *)ret = *(u8 *)left->metadata >> *(u8 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      *(i16 *)ret = *(i16 *)left->metadata >> *(i16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      *(u16 *)ret = *(u16 *)left->metadata >> *(u16 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      *(i32 *)ret = *(i32 *)left->metadata >> *(i32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      *(u32 *)ret = *(u32 *)left->metadata >> *(u32 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      *(i64 *)ret = *(i64 *)left->metadata >> *(i64 *)right->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      *(u64 *)ret = *(u64 *)left->metadata >> *(u64 *)right->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, ret, copyAstTree(left->type),
                      NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_PUTC: {
    putchar(*(u8 *)arguments[0]->metadata);
    return copyAstTree(&AST_TREE_VOID_VALUE);
  }
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY: {
    AstTree *path = arguments[0];
    char *str = u8ArrayToCString(path);
    void *dl = dynamicLibraryOpen(str);

    if (dl == NULL) {
      printLog("Can't open dl %s", str);
      UNREACHABLE;
    }
    free(str);

    AstTreeCLibrary *metadata = a404m_malloc(sizeof(*metadata));
    metadata->dl = dl;

    return newAstTree(AST_TREE_TOKEN_VALUE_C_LIBRARY, metadata,
                      &AST_TREE_C_LIBRARY_TYPE, NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION: {
    AstTree *library = arguments[0];
    AstTree *name = arguments[1];
    AstTree *funcType = arguments[2];

    AstTreeCFunction *metadata = a404m_malloc(sizeof(*metadata));
    metadata->library = copyAstTree(library);
    metadata->name = copyAstTree(name);
    metadata->funcType = copyAstTree(funcType);

    if (tree->type->token != AST_TREE_TOKEN_TYPE_FUNCTION) {
      UNREACHABLE;
    }

    AstTreeTypeFunction *function = tree->type->metadata;

    return newAstTree(AST_TREE_TOKEN_VALUE_C_FUNCTION, metadata,
                      copyAstTree(function->returnType), NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC: {
    AstTree *count = arguments[0];
    AstTree *type = arguments[1];

    const size_t sizeOfType = getSizeOfType(type);
    const size_t size = *(u64 *)count->metadata * sizeOfType;

    AstTreeRawValue *value = a404m_malloc(sizeof(void *));
    *(void **)value = stackAlloc(size, scope);

    AstTreeTypeFunction *tree_type = tree->type->metadata;

    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value,
                      copyAstTree(tree_type->returnType), NULL, NULL);
  }
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  default:
  }
  UNREACHABLE;
}

AstTree *runAstTreeCFunction(AstTree *tree, AstTree **arguments,
                             size_t arguments_size) {
  AstTreeCFunction *metadata = tree->metadata;
  AstTreeCLibrary *lib = metadata->library->metadata;
  char *name = u8ArrayToCString(metadata->name);
  AstTreeTypeFunction *funcType = metadata->funcType->metadata;

  void (*fun)() = dlsym(lib->dl, name);
  free(name);
  if (dlerror() != NULL) {
    UNREACHABLE;
  }

  if (funcType->arguments_size != arguments_size) {
    UNREACHABLE;
  }

  ffi_type *args[arguments_size];
  void *values[arguments_size];

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTreeTypeFunctionArgument arg = funcType->arguments[i];
    args[i] = toFFIType(arg.type);
    if (!typeIsEqual(arg.type, arguments[i]->type)) {
      printLog("%s %s", AST_TREE_TOKEN_STRINGS[arg.type->token],
               AST_TREE_TOKEN_STRINGS[arguments[i]->type->token]);
      UNREACHABLE;
    } else if (arguments[i]->token != AST_TREE_TOKEN_RAW_VALUE) {
      UNREACHABLE;
    }
    values[i] = arguments[i]->metadata;
  }

  ffi_cif cif;
  ffi_type *retType = toFFIType(funcType->returnType);
  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, arguments_size, retType, args) ==
      FFI_OK) {
    ffi_arg rc;
    ffi_call(&cif, fun, &rc, values);

    for (size_t i = 0; i < arguments_size; ++i) {
      deleteFFIType(args[i]);
    }
    deleteFFIType(retType);

    if (typeIsEqual(funcType->returnType, &AST_TREE_VOID_TYPE)) {
      return &AST_TREE_VOID_TYPE;
    } else {
      const size_t size = getSizeOfType(funcType->returnType);
      AstTreeRawValue *value = a404m_malloc(size);
      memcpy(value, &rc, size);
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value,
                        copyAstTree(funcType->returnType), NULL, NULL);
    }
  } else {
    UNREACHABLE;
  }

  return &AST_TREE_VOID_VALUE;
}

AstTree *runExpression(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                       bool isLeft, bool isComptime, u32 *breakCount,
                       bool *shouldContinue, bool needOwnership) {
  switch (expr->token) {
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = expr->metadata;
    AstTree *function =
        runExpression(metadata->function, scope, shouldRet, false, isComptime,
                      breakCount, shouldContinue, false);
    if (discontinue(*shouldRet, *breakCount)) {
      return function;
    }

    const size_t args_size = metadata->parameters_size;
    AstTree *args[args_size];

    AstTree *result;
    if (function->token == AST_TREE_TOKEN_FUNCTION) {
      AstTreeFunction *fun = function->metadata;

      for (size_t i = 0; i < args_size; ++i) {
        AstTreeVariable *function_arg = fun->arguments.data[i];
        AstTreeFunctionCallParam param = metadata->parameters[i];
        args[i] = getForVariable(param.value, scope, shouldRet, false,
                                 isComptime, breakCount, shouldContinue,
                                 function_arg->isLazy, false);
        if (discontinue(*shouldRet, *breakCount)) {
          astTreeDelete(function);
          for (size_t j = 0; j < i; ++j) {
            astTreeDelete(args[i]);
          }
          return args[i];
        }
      }
      result = runAstTreeFunction(function, args, args_size, scope, isComptime);
    } else if (function->token >= AST_TREE_TOKEN_BUILTIN_BEGIN &&
               function->token <= AST_TREE_TOKEN_BUILTIN_END) {
      for (size_t i = 0; i < args_size; ++i) {
        AstTreeFunctionCallParam param = metadata->parameters[i];
        if (function->token != AST_TREE_TOKEN_BUILTIN_TYPE_OF) {
          args[i] =
              getForVariable(param.value, scope, shouldRet, false, isComptime,
                             breakCount, shouldContinue, false, false);
          if (discontinue(*shouldRet, *breakCount)) {
            astTreeDelete(function);
            for (size_t j = 0; j < i; ++j) {
              astTreeDelete(args[i]);
            }
            return args[i];
          }
        } else {
          args[i] = param.value;
        }
      }
      result = runAstTreeBuiltin(function, scope, args);
      if (function->token != AST_TREE_TOKEN_BUILTIN_TYPE_OF) {
        for (size_t i = 0; i < args_size; ++i) {
          astTreeDelete(args[i]);
        }
      }
    } else if (function->token == AST_TREE_TOKEN_VALUE_C_FUNCTION) {
      for (size_t i = 0; i < args_size; ++i) {
        AstTreeFunctionCallParam param = metadata->parameters[i];
        args[i] =
            getForVariable(param.value, scope, shouldRet, false, isComptime,
                           breakCount, shouldContinue, false, false);
        if (discontinue(*shouldRet, *breakCount)) {
          astTreeDelete(function);
          for (size_t j = 0; j < i; ++j) {
            astTreeDelete(args[i]);
          }
          return args[i];
        }
      }
      result = runAstTreeCFunction(function, args, args_size);
      for (size_t i = 0; i < args_size; ++i) {
        astTreeDelete(args[i]);
      }
    } else {
      UNREACHABLE;
    }

    astTreeDelete(function);
    return result;
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreePureInfix *metadata = expr->metadata;
    if (metadata->left->token == AST_TREE_TOKEN_OPERATOR_DEREFERENCE) {
      AstTree *right =
          runExpression(metadata->right, scope, shouldRet, false, isComptime,
                        breakCount, shouldContinue, false);
      if (discontinue(*shouldRet, *breakCount)) {
        return right;
      }

      AstTree *l = (AstTreeSingleChild *)metadata->left->metadata;
      l = runExpression(l, scope, shouldRet, false, isComptime, breakCount,
                        shouldContinue, false);
      if (right->token != AST_TREE_TOKEN_RAW_VALUE) {
        UNREACHABLE;
      }
      if (l->token != AST_TREE_TOKEN_RAW_VALUE &&
          l->type->token != AST_TREE_TOKEN_OPERATOR_POINTER) {
        UNREACHABLE;
      }
      void **value = l->metadata;
      memcpy(*value, right->metadata, getSizeOfType(right->type));
      astTreeDelete(l);
      return right;
    } else {
      AstTree *l = runExpression(metadata->left, scope, shouldRet, true,
                                 isComptime, breakCount, shouldContinue, false);
      if (discontinue(*shouldRet, *breakCount)) {
        return l;
      }
      if (l->token == AST_TREE_TOKEN_VARIABLE) {
        AstTreeVariable *left = l->metadata;
        AstTree *right =
            runExpression(metadata->right, scope, shouldRet, false, isComptime,
                          breakCount, shouldContinue, false);
        if (discontinue(*shouldRet, *breakCount)) {
          astTreeDelete(l);
          return right;
        }
        runnerVariableSetValue(left, right, scope);
        astTreeDelete(l);
        return copyAstTree(left->value);
      } else if (l->token == AST_TREE_TOKEN_RAW_VALUE) {
        UNREACHABLE;
        AstTree *right =
            runExpression(metadata->right, scope, shouldRet, false, isComptime,
                          breakCount, shouldContinue, false);
        if (discontinue(*shouldRet, *breakCount)) {
          astTreeDelete(l);
          return right;
        }
        if (right->token != AST_TREE_TOKEN_RAW_VALUE) {
          NOT_IMPLEMENTED;
        }
        memcpy(l->metadata, right->metadata, getSizeOfType(l->type));
        astTreeDelete(right);
        return l;
      }
    }
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = expr->metadata;
    if (metadata->value != NULL) {
      AstTree *ret =
          runExpression(metadata->value, scope, shouldRet, false, isComptime,
                        breakCount, shouldContinue, true);
      *shouldRet = true;
      return ret;
    } else {
      *shouldRet = true;
      return &AST_TREE_VOID_VALUE;
    }
  }
  case AST_TREE_TOKEN_VARIABLE_DEFINE: {
    AstTreeVariable *variable = expr->metadata;
    AstTree *value;
    if (variable->isLazy) {
      value = copyAstTree(variable->initValue);
    } else {
      value = runExpression(variable->initValue, scope, shouldRet, false,
                            isComptime, breakCount, shouldContinue, false);
      if (discontinue(*shouldRet, *breakCount)) {
        return value;
      }
    }
    runnerVariableSetValue(variable, value, scope);
    return &AST_TREE_VOID_VALUE;
  }
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = expr->metadata;
    AstTree *condition =
        runExpression(metadata->condition, scope, shouldRet, false, isComptime,
                      breakCount, shouldContinue, false);
    if (discontinue(*shouldRet, *breakCount)) {
      return condition;
    }
    bool condi = *(AstTreeBool *)condition->metadata;
    astTreeDelete(condition);
    AstTree *ret;
    if (condi) {
      ret = runExpression(metadata->ifBody, scope, shouldRet, isLeft,
                          isComptime, breakCount, shouldContinue, false);
    } else if (metadata->elseBody != NULL) {
      ret = runExpression(metadata->elseBody, scope, shouldRet, isLeft,
                          isComptime, breakCount, shouldContinue, false);
    } else {
      ret = &AST_TREE_VOID_VALUE;
    }
    return ret;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = expr->metadata;
    while (!*shouldRet) {
      AstTree *condition =
          runExpression(metadata->condition, scope, shouldRet, false,
                        isComptime, breakCount, shouldContinue, false);
      if (discontinue(*shouldRet, *breakCount)) {
        return condition;
      }
      bool conti = *(AstTreeBool *)condition->metadata;
      astTreeDelete(condition);
      if (!conti) {
        break;
      }
      AstTree *ret =
          runExpression(metadata->body, scope, shouldRet, isLeft, isComptime,
                        breakCount, shouldContinue, false);
      if (*shouldRet) {
        return ret;
      }
      astTreeDelete(ret);
      if (*breakCount == 1 && *shouldContinue) {
        *breakCount -= 1;
        *shouldContinue = false;
      } else if (*breakCount != 0) {
        *breakCount -= 1;
        break;
      }
    }
    return &AST_TREE_VOID_VALUE;
  }
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *operand = expr->metadata;
    return runExpression((AstTree *)operand, scope, shouldRet, isLeft,
                         isComptime, breakCount, shouldContinue, false);
  }
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = expr->metadata;

    AstTree *ret = &AST_TREE_VOID_VALUE;
    for (size_t i = 0; i < metadata->expressions_size && !*shouldRet; ++i) {
      astTreeDelete(ret);
      ret = runExpression(metadata->expressions[i], scope, shouldRet,
                          i == metadata->expressions_size - 1 && isLeft,
                          isComptime, breakCount, shouldContinue, false);
      if (discontinue(*shouldRet, *breakCount)) {
        return ret;
      }
    }
    return ret;
  }
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
    return copyAstTree(expr);
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME: {
    AstTreeBool *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = isComptime;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata,
                      copyAstTree(&AST_TREE_BOOL_TYPE), expr->str_begin,
                      expr->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_ADDRESS: {
    AstTreeSingleChild *metadata = expr->metadata;
    if (metadata->token == AST_TREE_TOKEN_VARIABLE) {
      AstTreeVariable *variable = metadata->metadata;
      AstTreeRawValue *address = a404m_malloc(sizeof(void *));
      *(void **)address = variable->value;
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, address,
                        copyAstTree(expr->type), NULL, NULL);
    } else {
      printLog("%s", AST_TREE_TOKEN_STRINGS[metadata->token]);
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTree *operand =
        runExpression(metadata, scope, shouldRet, false, isComptime, breakCount,
                      shouldContinue, false);
    if (discontinue(*shouldRet, *breakCount)) {
      return operand;
    }
    if (operand->token == AST_TREE_TOKEN_RAW_VALUE) {
      if (operand->type->token != AST_TREE_TOKEN_OPERATOR_POINTER) {
        printLog("%s %p", AST_TREE_TOKEN_STRINGS[operand->type->token],
                 operand);
        UNREACHABLE;
      }
      AstTree *type =
          copyAstTree((AstTreeSingleChild *)operand->type->metadata);
      size_t size = getSizeOfType(type);
      AstTreeRawValue *value = a404m_malloc(size);
      memcpy(value, *(void **)operand->metadata, size);
      astTreeDelete(operand);
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value, type, NULL, NULL);
    } else if (operand->token == AST_TREE_TOKEN_VARIABLE) {
      AstTree *ret;
      if (isLeft) {
        ret = operand;
      } else {
        AstTreeVariable *variable = operand->metadata;
        ret = copyAstTree(variable->value);
        astTreeDelete(operand);
      }
      return ret;
    } else {
      printLog("%s", AST_TREE_TOKEN_STRINGS[operand->token]);
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = expr->metadata;
    if (isLeft) {
      return copyAstTree(expr);
    } else {
      if (variable->value == NULL) {
        UNREACHABLE;
      }
      if (variable->isLazy) {
        UNREACHABLE;
        AstTree *value =
            runExpression(variable->value, scope, shouldRet, false, isComptime,
                          breakCount, shouldContinue, false);
        runnerVariableSetValue(variable, value, scope);
      }
      return copyAstTree(variable->value);
    }
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = expr->metadata;
    AstTree *tree =
        runExpression(metadata->object, scope, shouldRet, false, isComptime,
                      breakCount, shouldContinue, false);
    if (discontinue(*shouldRet, *breakCount)) {
      return tree;
    }
    if (tree->token == AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
      /*
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
          AstTree *sizeTree = runExpression(
              array_metadata->parameters.data[0], scope, shouldRet, false,
              isComptime, breakCount, shouldContinue, false);
          if (discontinue(*shouldRet, *breakCount)) {
            return sizeTree;
          }
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
            copyAstTreeVariables(struc->variables, NULL, NULL, 0, false);

        for (size_t i = 0; i < newMetadata->variables.size; ++i) {
          AstTreeVariable *member = newMetadata->variables.data[i];
          if (!member->isConst) {
            runnerVariableSetValue(member,
                                   newAstTree(AST_TREE_TOKEN_VALUE_UNDEFINED,
                                              NULL, copyAstTree(member->type),
                                              variable->value->str_begin,
                                              variable->value->str_end),
                                   scope);
          }
        }

        runnerVariableSetValue(
            variable,
            newAstTree(AST_TREE_TOKEN_VALUE_OBJECT, newMetadata,
                       copyAstTree(variable->type),
                       variable->value->str_begin, variable->value->str_end),
            scope);
      }
      AstTreeObject *object = variable->value->metadata;
      AstTreeVariable *var = object->variables.data[metadata->member.index];
      if (isLeft) {
        return newAstTree(AST_TREE_TOKEN_VARIABLE, var,
                          copyAstTree(var->type), var->name_begin,
                          var->name_end);
      } else {
        return copyAstTree(var->value);
      }
    }
      */
      UNREACHABLE;
    } else if (tree->token == AST_TREE_TOKEN_RAW_VALUE) {
      if (tree->type->token == AST_TREE_TOKEN_TYPE_ARRAY) {
        u64 *tree_value = tree->metadata;

        u64 *value = a404m_malloc(sizeof(*value));
        *value = tree_value[metadata->member.index];
        astTreeDelete(tree);
        return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value,
                          copyAstTree(expr->type), NULL, NULL);
      }
      size_t index = 0;
      AstTreeStruct *type = tree->type->metadata;
      for (size_t i = 0; i < metadata->member.index; ++i) {
        index += getSizeOfType(type->variables.data[i]->type);
      }
      AstTree *t =
          copyAstTree(type->variables.data[metadata->member.index]->type);
      size_t size = getSizeOfType(t);
      AstTreeRawValue *value = a404m_malloc(size);
      memcpy(value, (u8 *)tree->metadata + index, size);
      astTreeDelete(tree);
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value, t, NULL, NULL);
    } else {
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    expr = copyAstTree(expr);
    AstTreeStruct *metadata = expr->metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *member = metadata->variables.data[i];
      AstTree *type = member->type;
      member->type =
          runExpression(member->type, scope, shouldRet, isLeft, isComptime,
                        breakCount, shouldContinue, false);
      astTreeDelete(type);
      if (discontinue(*shouldRet, *breakCount)) {
        astTreeDelete(expr);
        return member->type;
      }
    }
    return expr;
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    AstTreeSingleChild *metadata = expr->metadata;
    AstTreeSingleChild *newMetadata =
        runExpression(metadata, scope, shouldRet, isLeft, isComptime,
                      breakCount, shouldContinue, false);
    if (discontinue(*shouldRet, *breakCount)) {
      return newMetadata;
    }

    return newAstTree(AST_TREE_TOKEN_OPERATOR_POINTER, newMetadata,
                      copyAstTree(expr->type), expr->str_begin, expr->str_end);
  }
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER: {
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT: {
    AstTreeShapeShifterElement *metadata = expr->metadata;

    if (metadata->shapeShifter->token != AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
    }
    AstTreeVariable *variable = metadata->shapeShifter->metadata;
    if (variable->value->token != AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER) {
      UNREACHABLE;
    }

    AstTreeShapeShifter *shapeShifter = variable->value->metadata;
    AstTreeFunction *function =
        shapeShifter->generateds.functions[metadata->index];

    return newAstTree(AST_TREE_TOKEN_FUNCTION,
                      copyAstTreeFunction(function, NULL, NULL, 0, false),
                      copyAstTree(expr->type), expr->str_begin, expr->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_BREAK: {
    AstTreeLoopControl *metadata = expr->metadata;
    *breakCount = metadata->count;
    return copyAstTree(&AST_TREE_VOID_VALUE);
  }
  case AST_TREE_TOKEN_KEYWORD_CONTINUE: {
    AstTreeLoopControl *metadata = expr->metadata;
    *breakCount = metadata->count;
    *shouldContinue = true;
    return copyAstTree(&AST_TREE_VOID_VALUE);
  }
  case AST_TREE_TOKEN_NONE:
  }
  printLog("%s", AST_TREE_TOKEN_STRINGS[expr->token]);
  UNREACHABLE;
}

AstTree *getForVariable(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                        bool isLeft, bool isComptime, u32 *breakCount,
                        bool *shouldContinue, bool isLazy, bool needOwnership) {
  if (isLazy) {
    return copyAstTree(expr);
  } else {
    return runExpression(expr, scope, shouldRet, isLeft, isComptime, breakCount,
                         shouldContinue, needOwnership);
  }
}

bool discontinue(bool shouldRet, u32 breakCount) {
  return shouldRet || breakCount > 0;
}

AstTree *toRawValue(AstTree *value, AstTreeScope *scope) {
  switch (value->token) {
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NULL: {
    const size_t size = getSizeOfType(value->type);
    AstTreeRawValue *rawValue = a404m_malloc(size);
    memset(rawValue, 0, size);
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, rawValue,
                      copyAstTree(value->type), value->str_begin,
                      value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_UNDEFINED: {
    size_t size;
    AstTreeRawValue *rawValue;
    if (value->type->token == AST_TREE_TOKEN_TYPE_ARRAY) {
      struct Array {
        void *data;
        size_t size;
      };
      size = sizeof(struct Array);
      rawValue = a404m_malloc(size);
      struct Array *array = (void *)rawValue;

      AstTreeBracket *bracket = value->type->metadata;

      if (bracket->parameters.size != 1) {
        UNREACHABLE;
      }

      const size_t sizeOfType = getSizeOfType(bracket->operand);
      const size_t size =
          *(u64 *)bracket->parameters.data[0]->metadata * sizeOfType;

      array->data = stackAlloc(size, scope);
      array->size = *(u64 *)bracket->parameters.data[0]->metadata;
    } else {
      size = getSizeOfType(value->type);
      rawValue = a404m_malloc(size);
      memset(rawValue, 0, size);
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, rawValue,
                      copyAstTree(value->type), value->str_begin,
                      value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_INT: {
    const size_t size = getSizeOfType(value->type);
    AstTreeRawValue *rawValue = a404m_malloc(size);
    memcpy(rawValue, value->metadata, size);
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, rawValue,
                      copyAstTree(value->type), value->str_begin,
                      value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_FLOAT: {
    const size_t size = getSizeOfType(value->type);
    AstTreeRawValue *rawValue = a404m_malloc(size);
    switch (value->token) {
    case AST_TREE_TOKEN_TYPE_F16:
      *(f16 *)rawValue = *(f128 *)value->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F32:
      *(f32 *)rawValue = *(f128 *)value->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      *(f64 *)rawValue = *(f128 *)value->metadata;
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      *(f128 *)rawValue = *(f128 *)value->metadata;
      break;
    default:
      UNREACHABLE;
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, rawValue,
                      copyAstTree(value->type), value->str_begin,
                      value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_OBJECT: {
    const size_t size = getSizeOfType(value->type);
    AstTreeRawValue *rawValue = a404m_malloc(sizeof(void *) + sizeof(u64));
    AstTreeObject *object = value->metadata;
    if (value->type->token == AST_TREE_TOKEN_TYPE_ARRAY) {
      u8 *ptr = stackAlloc(size, scope);
      ((u64 *)rawValue)[0] = (u64)ptr;
      ((u64 *)rawValue)[1] = size;

      size_t filledSize = 0;
      for (size_t i = 0; i < object->items_size; ++i) {
        AstTreeObjectItem item = object->items[i];
        AstTree *itemValue = toRawValue(item.value, scope);
        if (itemValue == NULL) {
          itemValue = item.value;
        }
        size_t valueSize = getSizeOfType(itemValue->type);
        memcpy(ptr + filledSize, itemValue->metadata, valueSize);
        filledSize += valueSize;
        if (itemValue != item.value) {
          astTreeDelete(itemValue);
        }
      }
    } else {
      size_t filledSize = 0;
      for (size_t i = 0; i < object->items_size; ++i) {
        AstTreeObjectItem item = object->items[i];
        AstTree *itemValue = toRawValue(item.value, scope);
        if (itemValue == NULL) {
          itemValue = item.value;
        }
        size_t valueSize = getSizeOfType(itemValue->type);
        memcpy((u8 *)rawValue + filledSize, itemValue->metadata, valueSize);
        filledSize += valueSize;
        if (itemValue != item.value) {
          astTreeDelete(itemValue);
        }
      }
    }
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, rawValue,
                      copyAstTree(value->type), value->str_begin,
                      value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
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
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_NONE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  }
  return NULL;
}

AstTree *castTo(AstTree *tree, AstTree *to) {
  switch (tree->type->token) {
  case AST_TREE_TOKEN_TYPE_VOID: {
    const u8 value = 0;
    switch (to->token) {
    case AST_TREE_TOKEN_TYPE_I8:
      doCast(value, i8, to);
      break;
    case AST_TREE_TOKEN_TYPE_U8:
      doCast(value, u8, to);
      break;
    case AST_TREE_TOKEN_TYPE_I16:
      doCast(value, i16, to);
      break;
    case AST_TREE_TOKEN_TYPE_U16:
      doCast(value, u16, to);
      break;
    case AST_TREE_TOKEN_TYPE_I32:
      doCast(value, i32, to);
      break;
    case AST_TREE_TOKEN_TYPE_U32:
      doCast(value, u32, to);
      break;
    case AST_TREE_TOKEN_TYPE_I64:
      doCast(value, i64, to);
      break;
    case AST_TREE_TOKEN_TYPE_U64:
      doCast(value, u64, to);
      break;
#ifdef FLOAT_16_SUPPORT
    case AST_TREE_TOKEN_TYPE_F16:
      doCast(value, f16, to);
      break;
#endif
    case AST_TREE_TOKEN_TYPE_F32:
      doCast(value, f32, to);
      break;
    case AST_TREE_TOKEN_TYPE_F64:
      doCast(value, f64, to);
      break;
    case AST_TREE_TOKEN_TYPE_F128:
      doCast(value, f128, to);
      break;
    case AST_TREE_TOKEN_TYPE_BOOL:
      doCast(value, bool, to);
      break;
    default:
      UNREACHABLE;
    }
    break;
  }
  case AST_TREE_TOKEN_TYPE_I8:
    doCastAll(tree, i8, to);
    break;
  case AST_TREE_TOKEN_TYPE_U8:
    doCastAll(tree, u8, to);
    break;
  case AST_TREE_TOKEN_TYPE_I16:
    doCastAll(tree, i16, to);
    break;
  case AST_TREE_TOKEN_TYPE_U16:
    doCastAll(tree, u16, to);
    break;
  case AST_TREE_TOKEN_TYPE_I32:
    doCastAll(tree, i32, to);
    break;
  case AST_TREE_TOKEN_TYPE_U32:
    doCastAll(tree, u32, to);
    break;
  case AST_TREE_TOKEN_TYPE_I64:
    doCastAll(tree, i64, to);
    break;
  case AST_TREE_TOKEN_TYPE_U64:
    doCastAll(tree, u64, to);
    break;
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
    doCastAll(tree, f16, to);
    break;
#endif
  case AST_TREE_TOKEN_TYPE_F32:
    doCastAll(tree, f32, to);
    break;
  case AST_TREE_TOKEN_TYPE_F64:
    doCastAll(tree, f64, to);
    break;
  case AST_TREE_TOKEN_TYPE_F128:
    doCastAll(tree, f128, to);
    break;
  case AST_TREE_TOKEN_TYPE_BOOL:
    doCastAll(tree, bool, to);
    break;
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    if (to->token == AST_TREE_TOKEN_OPERATOR_POINTER) {
      void **value = a404m_malloc(sizeof(*value));
      *value = *(void **)tree->metadata;
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value, copyAstTree(to), NULL,
                        NULL);
    } else if (to->token == AST_TREE_TOKEN_TYPE_U64) {
      u64 *value = a404m_malloc(sizeof(*value));
      *value = *(u64 *)tree->metadata;
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value, copyAstTree(to), NULL,
                        NULL);
    } else if (to->token == AST_TREE_TOKEN_TYPE_I64) {
      i64 *value = a404m_malloc(sizeof(*value));
      *value = *(i64 *)tree->metadata;
      return newAstTree(AST_TREE_TOKEN_RAW_VALUE, value, copyAstTree(to), NULL,
                        NULL);
    }
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_NONE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  }
  printLog("%s", AST_TREE_TOKEN_STRINGS[tree->type->token]);
  UNREACHABLE;
}

ffi_type *toFFIType(AstTree *type) {
  switch (type->token) {
  case AST_TREE_TOKEN_TYPE_VOID:
    return &ffi_type_void;
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_U8:
    return &ffi_type_uint8;
  case AST_TREE_TOKEN_TYPE_I8:
    return &ffi_type_sint8;
  case AST_TREE_TOKEN_TYPE_I16:
    return &ffi_type_sint16;
  case AST_TREE_TOKEN_TYPE_U16:
    return &ffi_type_uint16;
  case AST_TREE_TOKEN_TYPE_I32:
    return &ffi_type_sint32;
  case AST_TREE_TOKEN_TYPE_U32:
    return &ffi_type_uint32;
  case AST_TREE_TOKEN_TYPE_I64:
    return &ffi_type_sint64;
  case AST_TREE_TOKEN_TYPE_U64:
    return &ffi_type_uint64;
  case AST_TREE_TOKEN_TYPE_F16:
    NOT_IMPLEMENTED;
  case AST_TREE_TOKEN_TYPE_F32:
    return &ffi_type_float;
  case AST_TREE_TOKEN_TYPE_F64:
    return &ffi_type_double;
  case AST_TREE_TOKEN_TYPE_F128:
    return &ffi_type_longdouble;
  case AST_TREE_TOKEN_OPERATOR_POINTER:
    return &ffi_type_pointer;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = type->metadata;

    ffi_type *type = a404m_malloc(sizeof(*type));
    *type = (ffi_type){
        .size = 0,
        .alignment = 0,
        .type = FFI_TYPE_STRUCT,
        .elements = a404m_malloc((metadata->variables.size + 1) *
                                 sizeof(*type->elements)),
    };

    for (size_t i = 0; i < metadata->variables.size; ++i) {
      type->elements[i] = toFFIType(metadata->variables.data[i]->type);
    }
    type->elements[metadata->variables.size] = NULL;

    return type;
  }
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_NONE:
  }
  printLog("%s", AST_TREE_TOKEN_STRINGS[type->token]);
  UNREACHABLE;
}

void deleteFFIType(ffi_type *type) {
  if (type->type == FFI_TYPE_STRUCT) {
    for (size_t i = 0; type->elements[i] != NULL; ++i) {
      deleteFFIType(type->elements[i]);
    }
    free(type->elements);
    free(type);
  }
}

void *stackAlloc(size_t size, AstTreeScope *scope) {
  size_t stackAllocation_capacity =
      a404m_malloc_usable_size(scope->stackAllocation) /
      sizeof(*scope->stackAllocation);
  if (scope->stackAllocation_size == stackAllocation_capacity) {
    stackAllocation_capacity += stackAllocation_capacity / 2 + 1;
    scope->stackAllocation = a404m_realloc(scope->stackAllocation,
                                           stackAllocation_capacity *
                                               sizeof(*scope->stackAllocation));
  }

  scope->stackAllocation[scope->stackAllocation_size] = a404m_malloc(size);
  scope->stackAllocation_size += 1;

  return scope->stackAllocation[scope->stackAllocation_size - 1];
}
