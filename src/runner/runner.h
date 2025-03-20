#pragma once

#include "compiler/ast-tree.h"
#include <stdint.h>

typedef struct RunnerVariable {
  AstTreeVariable *variable;
  AstTree *value;
} RunnerVariable;

typedef struct RunnerVariables {
  RunnerVariable **data;
  size_t size;
} RunnerVariables;

typedef struct RunnerVariablePages {
  RunnerVariables **data;
  size_t size;
} RunnerVariablePages;

void runnerVariablesDelete(RunnerVariables *variables);

void runnerVariablePush(RunnerVariables *variables, AstTreeVariable *variable);
void runnerVariableSetValue(RunnerVariablePages *pages,
                            AstTreeVariable *variable, AstTree *value);
AstTree *runnerVariableGetValue(RunnerVariablePages *pages,
                                AstTreeVariable *variable);

RunnerVariablePages initRootPages();
void destroyRootPages(RunnerVariablePages pages);

bool runAstTree(AstTreeRoot *root);

AstTree *runAstTreeFunction(AstTree *tree, AstTreeFunctionCallParam *arguments,
                            size_t arguments_size, RunnerVariablePages *pages);

AstTree *runExpression(AstTree *expr, RunnerVariablePages *pages,
                       bool *shouldRet);
