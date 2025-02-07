#include "runner.h"
#include "compiler/ast-tree.h"
#include "utils/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool runAstTree(AstTreeRoot *root) {
  constexpr char MAIN_STR[] = "main";
  constexpr size_t MAIN_STR_SIZE =
      (sizeof(MAIN_STR) / sizeof(*MAIN_STR)) - sizeof(*MAIN_STR);
  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    size_t name_size = variable->name_end - variable->name_begin;
    if (name_size == MAIN_STR_SIZE &&
        strncmp(variable->name_begin, MAIN_STR, MAIN_STR_SIZE) == 0 &&
        variable->value->token == AST_TREE_TOKEN_FUNCTION) {

      AstTree *main = variable->value;
      AstTreeFunction *mainFunction = main->metadata;
      return runAstTreeFunction(mainFunction) == &AST_TREE_VOID_VALUE;
    }
  }
  printLog("main function is not found");
  return false;
}

AstTree *runAstTreeFunction(AstTreeFunction *function) {
  for (size_t i = 0; i < function->scope.expressions_size; ++i) {
    AstTree expr = function->scope.expressions[i];
    switch (expr.token) {
    case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
      AstTreeSingleChild *metadata = expr.metadata;
      AstTree *tree = calcAstTreeValue(metadata);
      printf("%ld", (AstTreeU64)tree->metadata);
      astTreeDelete(tree);
    }
      continue;
    case AST_TREE_TOKEN_FUNCTION_CALL: {
      AstTree *ret = calcAstTreeValue(&expr);
      if (ret != &AST_TREE_VOID_VALUE) {
        astTreeDelete(ret);
      }
    }
      continue;
    case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
      AstTreeInfix *metadata = expr.metadata;
      if (metadata->left.token == AST_TREE_TOKEN_VARIABLE) {
        AstTreeVariable *left = metadata->left.metadata;
        astTreeDelete(left->value);
        left->value = calcAstTreeValue(&metadata->right);
      } else {
        UNREACHABLE;
      }
    }
      continue;
    case AST_TREE_TOKEN_KEYWORD_RETURN: {
      AstTreeReturn *metadata = expr.metadata;
      if (metadata->value != NULL) {
        return calcAstTreeValue(metadata->value);
      } else {
        return &AST_TREE_VOID_VALUE;
      }
    }
      continue;
    case AST_TREE_TOKEN_VARIABLE_DEFINE: {
      AstTreeVariable *variable = expr.metadata;
      variable->value = calcAstTreeValue(variable->value);
    }
      continue;
    case AST_TREE_TOKEN_OPERATOR_SUM:
    case AST_TREE_TOKEN_FUNCTION:
    case AST_TREE_TOKEN_TYPE_TYPE:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_TYPE_U64:
    case AST_TREE_TOKEN_VARIABLE:
    case AST_TREE_TOKEN_VALUE_U64:
    case AST_TREE_TOKEN_NONE:
    }
    printLog("%d", expr.token);
    UNREACHABLE;
  }

  return &AST_TREE_VOID_VALUE;
}

AstTree *calcAstTreeValue(AstTree *tree) {
  switch (tree->token) {
  case AST_TREE_TOKEN_VALUE_U64: {
    return deepCopyAstTree(tree);
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = tree->metadata;
    return calcAstTreeValue(variable->value);
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    if (metadata->parameters_size != 0) {
      UNREACHABLE;
    } else if (metadata->function->token == AST_TREE_TOKEN_VARIABLE) {
      AstTreeVariable *variable = metadata->function->metadata;
      return runAstTreeFunction(variable->value->metadata);
    } else {
      UNREACHABLE;
    }
  }
  case AST_TREE_TOKEN_OPERATOR_SUM: {
    AstTreeInfix *metadata = tree->metadata;
    AstTree *left = calcAstTreeValue(&metadata->left);
    AstTree *right = calcAstTreeValue(&metadata->right);

    if (left->type == &AST_TREE_U64_TYPE && right->type == &AST_TREE_U64_TYPE) {
      if (left->token == AST_TREE_TOKEN_VALUE_U64 &&
          right->token == AST_TREE_TOKEN_VALUE_U64) {
        left->metadata =
            (void *)((AstTreeU64)left->metadata + (AstTreeU64)right->metadata);
        astTreeDelete(right);
        return left;
      } else {
        UNREACHABLE;
      }
    }
  }
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *deepCopyAstTree(AstTree *tree) {
  switch (tree->token) {
  case AST_TREE_TOKEN_VALUE_U64:
    return newAstTree(tree->token, tree->metadata, copyAstTree(tree->type));
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}
