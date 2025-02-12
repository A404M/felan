#include "runner.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runnerVariablesDelete(RunnerVariables *variables) {
  for (size_t i = 0; i < variables->size; ++i) {
    if (variables->data[i]->value != NULL) {
      astTreeDelete(variables->data[i]->value);
    }
    free(variables->data[i]);
  }
  free(variables->data);
  free(variables);
}

void runnerVariablePush(RunnerVariables *variables, AstTreeVariable *variable) {
  size_t variables_size =
      a404m_malloc_usable_size(variables->data) / sizeof(*variables->data);
  if (variables_size == variables->size) {
    variables_size += variables_size / 2 + 1;
    variables->data = a404m_realloc(variables->data,
                                    variables_size * sizeof(*variables->data));
  }
  variables->data[variables->size] =
      a404m_malloc(sizeof(*variables->data[variables->size]));
  variables->data[variables->size]->variable = variable;
  variables->data[variables->size]->value = NULL;
  variables->size += 1;
}

void runnerVariableSetValue(RunnerVariablePages *pages,
                            AstTreeVariable *variable, AstTree *value) {
  if (variable->isConst) {
    printError(variable->name_begin, variable->name_end,
               "Can't assign to const");
    exit(1);
  }
  for (size_t i = 0; i < pages->size; ++i) {
    RunnerVariables *variables = pages->data[i];
    for (size_t j = 0; j < variables->size; ++j) {
      RunnerVariable *var = variables->data[j];
      if (var->variable == variable) {
        if (var->value != NULL) {
          astTreeDelete(var->value);
        }
        var->value = value;
        return;
      }
    }
  }

  UNREACHABLE;
}

AstTree *runnerVariableGetValue(RunnerVariablePages *pages,
                                AstTreeVariable *variable) {
  if (variable->isConst) {
    return variable->value;
  }
  for (size_t i = 0; i < pages->size; ++i) {
    RunnerVariables *variables = pages->data[i];
    for (size_t j = 0; j < variables->size; ++j) {
      RunnerVariable *var = variables->data[j];
      if (var->variable == variable) {
        if (var->value != NULL) {
          return var->value;
        } else {
          return var->variable->value;
        }
      }
    }
  }

  UNREACHABLE;
}

bool runAstTree(AstTreeRoot *root) {
  constexpr char MAIN_STR[] = "main";
  constexpr size_t MAIN_STR_SIZE =
      (sizeof(MAIN_STR) / sizeof(*MAIN_STR)) - sizeof(*MAIN_STR);

  RunnerVariablePages pages = {
      .data = a404m_malloc(sizeof(*pages.data)),
      .size = 1,
  };

  pages.data[pages.size - 1] =
      a404m_malloc(sizeof(*pages.data[pages.size - 1]));
  pages.data[pages.size - 1]->data = a404m_malloc(0);
  pages.data[pages.size - 1]->size = 0;

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (variable->isConst) {
      runnerVariablePush(pages.data[pages.size - 1], variable);
    }
  }

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    AstTree *variable_value = runnerVariableGetValue(&pages, variable);
    size_t name_size = variable->name_end - variable->name_begin;
    if (name_size == MAIN_STR_SIZE &&
        strncmp(variable->name_begin, MAIN_STR, MAIN_STR_SIZE) == 0 &&
        variable_value->token == AST_TREE_TOKEN_FUNCTION) {

      AstTree *main = variable_value;

      const bool ret =
          runAstTreeFunction(main, NULL, 0, &pages) == &AST_TREE_VOID_VALUE;
      runnerVariablesDelete(pages.data[pages.size - 1]);
      free(pages.data);
      return ret;
    }
  }
  runnerVariablesDelete(pages.data[pages.size - 1]);
  free(pages.data);
  printLog("main function is not found");
  return false;
}

AstTree *runAstTreeFunction(AstTree *tree, AstTree **arguments,
                            size_t arguments_size,
                            RunnerVariablePages *_pages) {
  AstTreeFunction *function = tree->metadata;

  RunnerVariablePages pages = {
      .data = a404m_malloc((_pages->size + 1) * sizeof(*pages.data)),
      .size = _pages->size + 1,
  };

  for (size_t i = 0; i < _pages->size; ++i) {
    pages.data[i] = _pages->data[i];
  }

  pages.data[pages.size - 1] =
      a404m_malloc(sizeof(*pages.data[pages.size - 1]));
  pages.data[pages.size - 1]->data = a404m_malloc(0);
  pages.data[pages.size - 1]->size = 0;

  for (size_t i = 0; i < arguments_size; ++i) {
    AstTreeVariable *variable = function->arguments.data[i];
    AstTree *value = calcAstTreeValue(arguments[i], &pages);
    runnerVariablePush(pages.data[pages.size - 1], variable);
    runnerVariableSetValue(&pages, variable, value);
  }

  for (size_t i = arguments_size; i < function->arguments.size; ++i) {
    AstTreeVariable *variable = function->arguments.data[i];
    runnerVariablePush(pages.data[pages.size - 1], variable);
  }

  for (size_t i = 0; i < function->scope.variables.size; ++i) {
    AstTreeVariable *variable = function->scope.variables.data[i];
    runnerVariablePush(pages.data[pages.size - 1], variable);
  }

  AstTree *ret = &AST_TREE_VOID_VALUE;

  for (size_t i = 0; i < function->scope.expressions_size; ++i) {
    AstTree *expr = function->scope.expressions[i];
  RUN:
    switch (expr->token) {
    case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
      AstTreeSingleChild *metadata = expr->metadata;
      AstTree *tree = calcAstTreeValue(metadata, &pages);
      printf("%lu", (AstTreeU64)tree->metadata);
      astTreeDelete(tree);
    }
      continue;
    case AST_TREE_TOKEN_FUNCTION_CALL: {
      AstTree *ret = calcAstTreeValue(expr, &pages);
      if (ret != &AST_TREE_VOID_VALUE) {
        astTreeDelete(ret);
      }
    }
      continue;
    case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
      AstTreeInfix *metadata = expr->metadata;
      if (metadata->left.token == AST_TREE_TOKEN_VARIABLE) {
        AstTreeVariable *left = metadata->left.metadata;
        runnerVariableSetValue(&pages, left,
                               calcAstTreeValue(&metadata->right, &pages));
      } else {
        UNREACHABLE;
      }
    }
      continue;
    case AST_TREE_TOKEN_KEYWORD_RETURN: {
      AstTreeReturn *metadata = expr->metadata;
      if (metadata->value != NULL) {
        ret = calcAstTreeValue(metadata->value, &pages);
      } else {
        ret = &AST_TREE_VOID_VALUE;
      }
      goto RETURN;
    }
      continue;
    case AST_TREE_TOKEN_VARIABLE_DEFINE: {
      AstTreeVariable *variable = expr->metadata;
      runnerVariableSetValue(&pages, variable, copyAstTree(variable->value));
    }
      continue;
    case AST_TREE_TOKEN_KEYWORD_IF: {
      AstTreeIf *metadata = expr->metadata;
      AstTree *tree = calcAstTreeValue(metadata->condition, &pages);
      if ((AstTreeBool)tree->metadata) {
        expr = metadata->body;
        goto RUN;
      }
      astTreeDelete(tree);
    }
      continue;
    case AST_TREE_TOKEN_OPERATOR_PLUS:
    case AST_TREE_TOKEN_OPERATOR_MINUS:
    case AST_TREE_TOKEN_OPERATOR_SUM:
    case AST_TREE_TOKEN_OPERATOR_SUB:
    case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
    case AST_TREE_TOKEN_OPERATOR_DIVIDE:
    case AST_TREE_TOKEN_OPERATOR_MODULO:
    case AST_TREE_TOKEN_FUNCTION:
    case AST_TREE_TOKEN_TYPE_TYPE:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_TYPE_U64:
    case AST_TREE_TOKEN_TYPE_BOOL:
    case AST_TREE_TOKEN_VARIABLE:
    case AST_TREE_TOKEN_VALUE_VOID:
    case AST_TREE_TOKEN_VALUE_U64:
    case AST_TREE_TOKEN_VALUE_BOOL:
    case AST_TREE_TOKEN_NONE:
    }
    printLog("Bad token %d", expr->token);
    UNREACHABLE;
  }

RETURN:
  runnerVariablesDelete(pages.data[pages.size - 1]);
  free(pages.data);

  return ret;
}

AstTree *calcAstTreeValue(AstTree *tree, RunnerVariablePages *pages) {
  switch (tree->token) {
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VALUE_BOOL:
    return deepCopyAstTree(tree);
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = tree->metadata;
    return calcAstTreeValue(runnerVariableGetValue(pages, variable), pages);
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    if (metadata->function->token == AST_TREE_TOKEN_VARIABLE) {
      AstTreeVariable *variable = metadata->function->metadata;
      return runAstTreeFunction(runnerVariableGetValue(pages, variable),
                                metadata->parameters, metadata->parameters_size,
                                pages);
    } else {
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO: {
    AstTreeInfix *metadata = tree->metadata;
    AstTree *left = calcAstTreeValue(&metadata->left, pages);
    AstTree *right = calcAstTreeValue(&metadata->right, pages);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      if (left->token == AST_TREE_TOKEN_VALUE_U64 &&
          right->token == AST_TREE_TOKEN_VALUE_U64) {
        switch (tree->token) {
        case AST_TREE_TOKEN_OPERATOR_SUM:
          left->metadata = (void *)((AstTreeU64)left->metadata +
                                    (AstTreeU64)right->metadata);
          break;
        case AST_TREE_TOKEN_OPERATOR_SUB:
          left->metadata = (void *)((AstTreeU64)left->metadata -
                                    (AstTreeU64)right->metadata);
          break;
        case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
          left->metadata = (void *)((AstTreeU64)left->metadata *
                                    (AstTreeU64)right->metadata);
          break;
        case AST_TREE_TOKEN_OPERATOR_DIVIDE:
          left->metadata = (void *)((AstTreeU64)left->metadata /
                                    (AstTreeU64)right->metadata);
          break;
        case AST_TREE_TOKEN_OPERATOR_MODULO:
          left->metadata = (void *)((AstTreeU64)left->metadata %
                                    (AstTreeU64)right->metadata);
          break;
        default:
          UNREACHABLE;
        }
        astTreeDelete(right);
        return left;
      } else {
        UNREACHABLE;
      }
    }
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeSingleChild *metadata = tree->metadata;
    if (typeIsEqual(metadata->type, &AST_TREE_U64_TYPE)) {
      AstTree *operand = calcAstTreeValue(metadata, pages);
      if (operand->token == AST_TREE_TOKEN_VALUE_U64) {
        switch (tree->token) {
        case AST_TREE_TOKEN_OPERATOR_PLUS:
          operand->metadata = (void *)(+(AstTreeU64)operand->metadata);
          break;
        case AST_TREE_TOKEN_OPERATOR_MINUS:
          operand->metadata = (void *)(-(AstTreeU64)operand->metadata);
          break;
        default:
          UNREACHABLE;
        }
        return operand;
      } else {
        UNREACHABLE;
      }
    } else {
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *deepCopyAstTree(AstTree *tree) {
  switch (tree->token) {
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VALUE_BOOL:
    return newAstTree(tree->token, tree->metadata, copyAstTree(tree->type),
                      NULL, NULL);
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}
