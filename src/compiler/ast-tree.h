#pragma once

#include "compiler/parser.h"
#include <stddef.h>
#include <stdint.h>

typedef enum AstTreeToken {
  AST_TREE_TOKEN_FUNCTION,

  AST_TREE_TOKEN_KEYWORD_PRINT_U64,
  AST_TREE_TOKEN_KEYWORD_RETURN,
  AST_TREE_TOKEN_KEYWORD_IF,

  AST_TREE_TOKEN_TYPE_TYPE,
  AST_TREE_TOKEN_TYPE_FUNCTION,
  AST_TREE_TOKEN_TYPE_VOID,
  AST_TREE_TOKEN_TYPE_U64,
  AST_TREE_TOKEN_TYPE_BOOL,

  AST_TREE_TOKEN_FUNCTION_CALL,
  AST_TREE_TOKEN_VARIABLE,
  AST_TREE_TOKEN_VARIABLE_DEFINE,
  AST_TREE_TOKEN_VALUE_VOID,
  AST_TREE_TOKEN_VALUE_U64,
  AST_TREE_TOKEN_VALUE_BOOL,

  AST_TREE_TOKEN_OPERATOR_ASSIGN,
  AST_TREE_TOKEN_OPERATOR_PLUS,
  AST_TREE_TOKEN_OPERATOR_MINUS,
  AST_TREE_TOKEN_OPERATOR_SUM,
  AST_TREE_TOKEN_OPERATOR_SUB,
  AST_TREE_TOKEN_OPERATOR_MULTIPLY,
  AST_TREE_TOKEN_OPERATOR_DIVIDE,
  AST_TREE_TOKEN_OPERATOR_MODULO,

  AST_TREE_TOKEN_SCOPE,

  AST_TREE_TOKEN_NONE,
} AstTreeToken;

extern const char *AST_TREE_TOKEN_STRINGS[];

typedef struct AstTree {
  AstTreeToken token;
  void *metadata;
  struct AstTree *type;
  char *str_begin;
  char *str_end;
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
  AstTree **expressions;
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

typedef bool AstTreeBool;

typedef AstTree AstTreeSingleChild;

typedef struct AstTreeInfix {
  AstTree left;
  AstTree right;
} AstTreeInfix;

typedef struct AstTreeReturn {
  AstTree *value;
} AstTreeReturn;

typedef struct AstTreeIf {
  AstTree *condition;
  AstTree *ifBody;
  AstTree *elseBody;
} AstTreeIf;

typedef struct AstTreeHelper {
  AstTreeVariables **variables;
  size_t variables_size;
  AstTreeVariable *variable;
  AstTreeVariables *globalDeps;
} AstTreeHelper;

void astTreePrint(const AstTree *tree, int indent);
void astTreeRootPrint(const AstTreeRoot *root);

void astTreeDestroy(AstTree tree);
void astTreeVariableDestroy(AstTreeVariable variable);
void astTreeVariableDelete(AstTreeVariable *variable);
void astTreeDelete(AstTree *tree);
void astTreeRootDelete(AstTreeRoot *root);

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type,
                    char *str_begin, char *str_end);
AstTree *copyAstTree(AstTree *tree);
AstTreeVariables copyAstTreeVariables(AstTreeVariables variables);

AstTreeRoot *makeAstTree(ParserNode *parsedRoot);

bool pushVariable(AstTreeHelper *helper, AstTreeVariables *variables,
                  AstTreeVariable *variable);
AstTreeVariable *getVariable(AstTreeHelper *helper, char *name_begin,
                             char *name_end);

AstTree *astTreeParse(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseFunction(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseTypeFunction(ParserNode *parserNode,
                                  AstTreeHelper *helper);
AstTree *astTreeParseFunctionCall(ParserNode *parserNode,
                                  AstTreeHelper *helper);
AstTree *astTreeParseIdentifier(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParsePrintU64(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseReturn(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseBinaryOperator(ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token);
AstTree *astTreeParseUnaryOperator(ParserNode *parserNode,
                                   AstTreeHelper *helper, AstTreeToken token);
bool astTreeParseConstant(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseVariable(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseIf(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseCurlyBracket(ParserNode *parserNode, AstTreeHelper *helper);

AstTreeFunction *getFunction(AstTree *value);
bool isConst(AstTree *value);
AstTree *makeTypeOf(AstTree *value);
bool typeIsEqual(const AstTree *type0, const AstTree *type1);

bool isCircularDependencies(AstTreeHelper *helper, AstTreeVariable *variable,
                            AstTree *tree);

bool setAllTypesRoot(AstTreeRoot *root, AstTreeHelper *helper);
bool setAllTypes(AstTree *tree, AstTreeFunction *function);
bool setTypesFunction(AstTree *tree);
bool setTypesPrintU64(AstTree *tree);
bool setTypesReturn(AstTree *tree, AstTreeFunction *function);
bool setTypesTypeFunction(AstTree *tree);
bool setTypesFunctionCall(AstTree *tree);
bool setTypesVariable(AstTree *tree);
bool setTypesOperatorAssign(AstTree *tree);
bool setTypesOperatorInfix(AstTree *tree);
bool setTypesOperatorUnary(AstTree *tree);
bool setTypesVariableDefine(AstTree *tree);
bool setTypesIf(AstTree *tree,AstTreeFunction *function);
bool setTypesScope(AstTree *tree,AstTreeFunction *function);

bool setTypesAstVariable(AstTreeVariable *variable);
bool setTypesAstInfix(AstTreeInfix *infix);

bool astTreeCleanRoot(AstTreeRoot *root);
bool astTreeClean(AstTree *tree);
bool astTreeCleanFunction(AstTree *tree);
bool astTreeCleanVariable(AstTree *tree);
bool astTreeCleanAstVariable(AstTreeVariable *variable);

size_t astTreeTypeSize(AstTree tree);
