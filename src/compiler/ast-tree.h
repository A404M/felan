#pragma once

#include "compiler/parser.h"
#include <stddef.h>

typedef enum AstTreeToken {
  AST_TREE_TOKEN_FUNCTION,
  AST_TREE_TOKEN_KEYWORD_PRINT,
  AST_TREE_TOKEN_TYPE_FUNCTION,
  AST_TREE_TOKEN_TYPE_VOID,
  AST_TREE_TOKEN_NONE,
} AstTreeToken;

typedef struct AstTree {
  AstTreeToken token;
  void *metadata;
} AstTree;

typedef struct AstTreeVariable {
  char *name_begin;
  char *name_end;
  AstTree *type;
  AstTree *value;
} AstTreeVariable;

typedef struct AstTreeVariables {
  AstTreeVariable **data;
  size_t size;
} AstTreeVariables;

typedef struct AstTreeRoot {
  AstTreeVariables variables;
} AstTreeRoot;

typedef struct AstTreeScope {
  AstTreeVariable *variables;
  size_t variables_size;
  AstTree *expressions;
  size_t expressions_size;
} AstTreeScope;

typedef struct AstTreeFunction {
  AstTreeVariable *arguments;
  size_t arguments_size;
  AstTreeScope scope;
  AstTree *returnType;
} AstTreeFunction;

typedef struct AstTreeTypeFunction {
  AstTree **arguments;
  size_t arguments_size;
  AstTree *returnType;
} AstTreeTypeFunction;

extern const char *AST_TREE_TOKEN_STRINGS[];

void astTreePrint(const AstTree *tree, int indent);
void astTreeRootPrint(const AstTreeRoot *root);

void astTreeDestroy(AstTree tree);
void astTreeDelete(AstTree *tree);
void astTreeRootDelete(AstTreeRoot *root);

AstTree *newAstTree(AstTreeToken token, void *metadata);

AstTreeRoot *makeAstTree(ParserNode *parsedRoot);

void pushVariable(AstTreeVariables *variables, size_t *variables_size,
                  AstTreeVariable *variable);

AstTree *astTreeParse(ParserNode *parserNode, AstTreeVariables *variables,
                      size_t variables_size);

AstTree *astTreeParseFunction(ParserNode *parserNode,
                              AstTreeVariables *variables,
                              size_t variables_size);

AstTree *astTreeParseTypeFunction(ParserNode *parserNode,
                                  AstTreeVariables *variables,
                                  size_t variables_size);

bool hasTypeOf(AstTree *value, AstTree *type);
bool typeIsEqual(AstTree *type0, AstTree *type1);
