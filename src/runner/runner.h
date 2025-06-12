#pragma once

#include "compiler/ast-tree.h"
#include <ffi.h>

typedef struct ArrayValueStruct {
  void *data;
  size_t size;
} ArrayValueStruct;

void runnerVariableSetValue(AstTreeVariable *variable, AstTree *value,
                            AstTreeScope *scope);
void runnerVariableSetValueWihtoutConstCheck(AstTreeVariable *variable,
                                             AstTree *value,
                                             AstTreeScope *scope);
AstTree *runnerVariableGetValue(AstTreeVariable *variable);

bool runAstTree(AstTreeRoots roots);

AstTree *runAstTreeFunction(AstTree *tree, AstTree **arguments,
                            size_t arguments_size, AstTreeScope *scope,
                            bool isComptime);

AstTree *runAstTreeBuiltin(AstTree *tree, AstTreeScope *scope,
                           AstTree **arguments);

AstTree *runAstTreeCFunction(AstTree *tree, AstTree **arguments,
                             size_t arguments_size, AstTreeScope *scope);

AstTree *runExpression(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                       bool isLeft, bool isComptime, u32 *breakCount,
                       bool *shouldContinue);

bool discontinue(bool shouldRet, u32 breakCount);

AstTree *toRawValue(AstTree *value, AstTreeScope *scope);
bool canBeRaw(AstTree *type);
AstTree *fromRawValue(AstTree *value);

AstTree *castTo(AstTree *value, AstTree *to);

ffi_type *toFFIType(AstTree *type);
void deleteFFIType(ffi_type *type);

void *stackAlloc(size_t size, AstTreeScope *scope);
