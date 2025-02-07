#pragma once

#include "compiler/parser.h"
#include <stddef.h>
#include <stdint.h>

typedef enum AstTreeToken {
  AST_TREE_TOKEN_FUNCTION,

  AST_TREE_TOKEN_KEYWORD_PRINT_U64,

  AST_TREE_TOKEN_TYPE_TYPE,
  AST_TREE_TOKEN_TYPE_FUNCTION,
  AST_TREE_TOKEN_TYPE_VOID,
  AST_TREE_TOKEN_TYPE_U64,

  AST_TREE_TOKEN_FUNCTION_CALL,
  AST_TREE_TOKEN_VARIABLE,
  AST_TREE_TOKEN_VARIABLE_DEFINE,
  AST_TREE_TOKEN_VALUE_U64,

  AST_TREE_TOKEN_OPERATOR_ASSIGN,
  AST_TREE_TOKEN_OPERATOR_SUM,

  AST_TREE_TOKEN_NONE,
} AstTreeToken;

extern const char *AST_TREE_TOKEN_STRINGS[];

typedef struct AstTree {
  AstTreeToken token;
  void *metadata;
  struct AstTree *type;
} AstTree;

extern AstTree AST_TREE_TYPE_TYPE;
extern AstTree AST_TREE_VOID_TYPE;
extern AstTree AST_TREE_U64_TYPE;
extern AstTree AST_TREE_VOID_VALUE;

typedef struct AstTreeVariable {
  char *name_begin;
  char *name_end;
  AstTree *type;
  AstTree *value;
  bool isConst;
} AstTreeVariable;

typedef struct AstTreeVariables {
  AstTreeVariable **data;
  size_t size;
} AstTreeVariables;

typedef struct AstTreeRoot {
  AstTreeVariables variables;
} AstTreeRoot;

typedef struct AstTreeScope {
  AstTreeVariables variables;
  AstTree *expressions;
  size_t expressions_size;
} AstTreeScope;

typedef struct AstTreeFunction {
  AstTreeVariables arguments;
  AstTreeScope scope;
  AstTree *returnType;
} AstTreeFunction;

typedef struct AstTreeTypeFunction {
  AstTree **arguments;
  size_t arguments_size;
  AstTree *returnType;
} AstTreeTypeFunction;

typedef struct AstTreeFunctionCall {
  AstTree *function;
  AstTree **parameters;
  size_t parameters_size;
} AstTreeFunctionCall;

typedef uint64_t AstTreeU64;

typedef AstTree AstTreeSingleChild;

typedef struct AstTreeInfix {
  AstTree left;
  AstTree right;
} AstTreeInfix;

void astTreePrint(const AstTree *tree, int indent);
void astTreeRootPrint(const AstTreeRoot *root);

void astTreeDestroy(AstTree tree);
void astTreeVariableDestroy(AstTreeVariable variable);
void astTreeVariableDelete(AstTreeVariable *variable);
void astTreeDelete(AstTree *tree);
void astTreeRootDelete(AstTreeRoot *root);

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type);
AstTree *copyAstTree(AstTree *tree);

AstTreeRoot *makeAstTree(ParserNode *parsedRoot);

bool pushVariable(AstTreeVariables *variables, AstTreeVariable *variable);
AstTreeVariable *getVariable(AstTreeVariables **variables,
                             size_t variables_size, char *name_begin,
                             char *name_end);

AstTree *astTreeParse(ParserNode *parserNode, AstTreeVariables **variables,
                      size_t variables_size);

AstTree *astTreeParseFunction(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size);

AstTree *astTreeParseTypeFunction(ParserNode *parserNode,
                                  AstTreeVariables **variables,
                                  size_t variables_size);

AstTree *astTreeParseFunctionCall(ParserNode *parserNode,
                                  AstTreeVariables **variables,
                                  size_t variables_size);

AstTree *astTreeParseIdentifier(ParserNode *parserNode,
                                AstTreeVariables **variables,
                                size_t variables_size);

AstTree *astTreeParsePrintU64(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size);

AstTree *astTreeParseAssign(ParserNode *parserNode,
                            AstTreeVariables **variables,
                            size_t variables_size);

AstTree *astTreeParseSum(ParserNode *parserNode, AstTreeVariables **variables,
                         size_t variables_size);

bool astTreeParseConstant(ParserNode *parserNode, AstTreeVariables **variables,
                          size_t variables_size);

AstTree *astTreeParseVariable(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size);

AstTreeFunction *getFunction(AstTree *value);
bool isConst(AstTree *value);
AstTree *makeTypeOf(AstTree *value);
bool typeIsEqual(const AstTree *type0, const AstTree *type1);

bool setAllTypesRoot(AstTreeRoot *root);
bool setAllTypes(AstTree *tree);
bool setTypesFunction(AstTree *tree);
bool setTypesPrintU64(AstTree *tree);
bool setTypesTypeFunction(AstTree *tree);
bool setTypesFunctionCall(AstTree *tree);
bool setTypesVariable(AstTree *tree);
bool setTypesOperatorAssign(AstTree *tree);
bool setTypesOperatorSum(AstTree *tree);

bool setTypesAstVariable(AstTreeVariable *variable);
bool setTypesAstInfix(AstTreeInfix *infix);

bool astTreeCleanRoot(AstTreeRoot *root);
bool astTreeClean(AstTree *tree);
bool astTreeCleanFunction(AstTree *tree);
bool astTreeCleanVariable(AstTree *tree);
bool astTreeCleanAstVariable(AstTreeVariable *variable);

size_t astTreeTypeSize(AstTree tree);
