#pragma once

#include "compiler/ast-tree.h"
#include <stdint.h>

bool runAstTree(AstTreeRoot *root);

AstTree *runAstTreeFunction(AstTreeFunction *function, AstTree **arguments,
                            size_t arguments_size);

AstTree *calcAstTreeValue(AstTree *tree);
AstTree *deepCopyAstTree(AstTree *tree);
