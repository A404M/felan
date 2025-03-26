#pragma once

#include "compiler/ast-tree.h"
#include <stdint.h>

void runnerVariableSetValue(AstTreeVariable *variable,AstTree *value);

bool runAstTree(AstTreeRoot *root);

AstTree *runAstTreeFunction(AstTree *tree, AstTreeFunctionCallParam *arguments,
                            size_t arguments_size);

AstTree *runExpression(AstTree *expr, bool *shouldRet);
