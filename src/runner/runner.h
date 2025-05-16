#pragma once

#include "compiler/ast-tree.h"

void runnerVariableSetValue(AstTreeVariable *variable, AstTree *value);
void runnerVariableSetValueWihtoutConstCheck(AstTreeVariable *variable,
                                             AstTree *value);
AstTree *runnerVariableGetValue(AstTreeVariable *variable);

bool runAstTree(AstTreeRoots roots);

AstTree *runAstTreeFunction(AstTree *tree, AstTreeFunctionCallParam *arguments,
                            size_t arguments_size, bool isComptime);

AstTree *runAstTreeBuiltin(AstTree *tree, AstTreeScope *scope,
                           AstTreeFunctionCallParam *arguments,
                           size_t arguments_size, bool isComptime);

AstTree *runExpression(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                       bool isLeft, bool isComptime, u32 *breakCount);
