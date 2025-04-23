#pragma once

#include "compiler/ast-tree.h"
#include <stdint.h>

void runnerVariableSetValue(AstTreeVariable *variable, AstTree *value);
void runnerVariableSetValueWihtoutConstCheck(AstTreeVariable *variable,
                                             AstTree *value);

bool runAstTree(AstTreeRoots roots);

AstTree *runAstTreeFunction(AstTree *tree, AstTreeFunctionCallParam *arguments,
                            size_t arguments_size);

AstTree *runAstTreeBuiltin(AstTree *tree, AstTreeScope *scope,
                           AstTreeFunctionCallParam *arguments,
                           size_t arguments_size);

AstTree *runExpression(AstTree *expr, AstTreeScope *scope, bool *shouldRet,
                       bool isLeft);
