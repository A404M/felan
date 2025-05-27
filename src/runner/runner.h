#pragma once

#include "compiler/ast-tree.h"
#include <ffi.h>

void runnerVariableSetValue(AstTreeVariable *variable, AstTree *value);
void runnerVariableSetValueWihtoutConstCheck(AstTreeVariable *variable,
                                             AstTree *value);
AstTree *runnerVariableGetValue(AstTreeVariable *variable);

bool runAstTree(AstTreeRoots roots);

AstTree *runAstTreeFunction(AstTree *tree, AstTree **arguments,
                            size_t arguments_size, bool isComptime);

AstTree *runAstTreeBuiltin(AstTree *tree, AstTreeScope *scope,
                           AstTree **arguments);

AstTree *runAstTreeCFunction(AstTree *tree, AstTree **arguments,
                             size_t arguments_size);

AstTree *runExpression(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                       bool isLeft, bool isComptime, u32 *breakCount,
                       bool *shouldContinue,bool needOwnership);

AstTree *getForVariable(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                        bool isLeft, bool isComptime, u32 *breakCount,
                        bool *shouldContinue, bool isLazy,bool needOwnership);

bool discontinue(bool shouldRet, u32 breakCount);

AstTree *toRawValue(AstTree *value);

AstTree *castTo(AstTree *value, AstTree *to);

ffi_type *toFFIType(AstTree *type);
void deleteFFIType(ffi_type *type);
