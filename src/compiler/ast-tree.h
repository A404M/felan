#pragma once

#include "compiler/parser.h"
#include <stddef.h>
#include <stdint.h>

typedef enum AstTreeToken {
  AST_TREE_TOKEN_FUNCTION,

  AST_TREE_TOKEN_KEYWORD_PRINT_U64,
  AST_TREE_TOKEN_KEYWORD_RETURN,
  AST_TREE_TOKEN_KEYWORD_IF,
  AST_TREE_TOKEN_KEYWORD_WHILE,
  AST_TREE_TOKEN_KEYWORD_COMPTIME,

  AST_TREE_TOKEN_TYPE_FUNCTION,
  AST_TREE_TOKEN_TYPE_TYPE,
  AST_TREE_TOKEN_STATIC_VARS_BEGIN = AST_TREE_TOKEN_TYPE_TYPE,
  AST_TREE_TOKEN_TYPE_VOID,
  AST_TREE_TOKEN_TYPE_I8,
  AST_TREE_TOKEN_TYPE_U8,
  AST_TREE_TOKEN_TYPE_I16,
  AST_TREE_TOKEN_TYPE_U16,
  AST_TREE_TOKEN_TYPE_I32,
  AST_TREE_TOKEN_TYPE_U32,
  AST_TREE_TOKEN_TYPE_I64,
  AST_TREE_TOKEN_TYPE_U64,
  AST_TREE_TOKEN_TYPE_F16,
  AST_TREE_TOKEN_TYPE_F32,
  AST_TREE_TOKEN_TYPE_F64,
  AST_TREE_TOKEN_TYPE_F128,
  AST_TREE_TOKEN_TYPE_BOOL,
  AST_TREE_TOKEN_VALUE_VOID,
  AST_TREE_TOKEN_STATIC_VARS_END = AST_TREE_TOKEN_VALUE_VOID,

  AST_TREE_TOKEN_FUNCTION_CALL,
  AST_TREE_TOKEN_VARIABLE,
  AST_TREE_TOKEN_VARIABLE_DEFINE,
  AST_TREE_TOKEN_VALUE_NULL,
  AST_TREE_TOKEN_VALUE_INT,
  AST_TREE_TOKEN_VALUE_FLOAT,
  AST_TREE_TOKEN_VALUE_BOOL,

  AST_TREE_TOKEN_OPERATOR_ASSIGN,
  AST_TREE_TOKEN_OPERATOR_PLUS,
  AST_TREE_TOKEN_OPERATOR_MINUS,
  AST_TREE_TOKEN_OPERATOR_SUM,
  AST_TREE_TOKEN_OPERATOR_SUB,
  AST_TREE_TOKEN_OPERATOR_MULTIPLY,
  AST_TREE_TOKEN_OPERATOR_DIVIDE,
  AST_TREE_TOKEN_OPERATOR_MODULO,
  AST_TREE_TOKEN_OPERATOR_EQUAL,
  AST_TREE_TOKEN_OPERATOR_NOT_EQUAL,
  AST_TREE_TOKEN_OPERATOR_GREATER,
  AST_TREE_TOKEN_OPERATOR_SMALLER,
  AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL,
  AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL,
  AST_TREE_TOKEN_OPERATOR_POINTER,
  AST_TREE_TOKEN_OPERATOR_ADDRESS,
  AST_TREE_TOKEN_OPERATOR_DEREFERENCE,

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
extern AstTree AST_TREE_BOOL_TYPE;
extern AstTree AST_TREE_I8_TYPE;
extern AstTree AST_TREE_U8_TYPE;
extern AstTree AST_TREE_I16_TYPE;
extern AstTree AST_TREE_U16_TYPE;
extern AstTree AST_TREE_I32_TYPE;
extern AstTree AST_TREE_U32_TYPE;
extern AstTree AST_TREE_I64_TYPE;
extern AstTree AST_TREE_U64_TYPE;
extern AstTree AST_TREE_F16_TYPE;
extern AstTree AST_TREE_F32_TYPE;
extern AstTree AST_TREE_F64_TYPE;
extern AstTree AST_TREE_F128_TYPE;
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

typedef struct AstTreeFunctionCallParam {
  char *nameBegin;
  char *nameEnd;
  AstTree *value;
} AstTreeFunctionCallParam;

typedef struct AstTreeFunctionCall {
  AstTree *function;
  AstTreeFunctionCallParam *parameters;
  size_t parameters_size;
} AstTreeFunctionCall;

typedef u64 AstTreeInt;

typedef f128 AstTreeFloat;

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

typedef struct AstTreeWhile {
  AstTree *condition;
  AstTree *body;
} AstTreeWhile;

typedef struct AstTreeHelper {
  AstTreeVariables **variables;
  size_t variables_size;
  AstTreeVariable *variable;
  AstTreeVariables *globalDeps;
} AstTreeHelper;

typedef struct AstTreeSetTypesHelper {
  AstTree *lookingType;
  AstTreeHelper *treeHelper;
} AstTreeSetTypesHelper;

void astTreePrint(const AstTree *tree, int indent);
void astTreeRootPrint(const AstTreeRoot *root);

void astTreeDestroy(AstTree tree);
void astTreeVariableDestroy(AstTreeVariable variable);
void astTreeVariableDelete(AstTreeVariable *variable);
void astTreeDelete(AstTree *tree);
bool astTreeShouldDelete(AstTree *tree);
void astTreeRootDelete(AstTreeRoot *root);

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type,
                    char *str_begin, char *str_end);
AstTree *copyAstTree(AstTree *tree);
AstTree *copyAstTreeBack(AstTree *tree, AstTreeVariables oldVariables[],
                         AstTreeVariables newVariables[],
                         size_t variables_size);
AstTreeVariables copyAstTreeVariables(AstTreeVariables variables,
                                      AstTreeVariables oldVariables[],
                                      AstTreeVariables newVariables[],
                                      size_t variables_size);

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
AstTree *astTreeParseValue(ParserNode *parserNode, AstTreeToken token,
                           size_t metadata_size);
AstTree *astTreeParseKeyword(ParserNode *parserNode, AstTreeToken token);
AstTree *astTreeParsePrintU64(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseReturn(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseBinaryOperator(ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token);
AstTree *astTreeParseUnaryOperator(ParserNode *parserNode,
                                   AstTreeHelper *helper, AstTreeToken token);
AstTree *astTreeParseOperateAssignOperator(ParserNode *parserNode,
                                           AstTreeHelper *helper,
                                           AstTreeToken token);
bool astTreeParseConstant(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseVariable(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseIf(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseWhile(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseComptime(ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseCurlyBracket(ParserNode *parserNode,
                                  AstTreeHelper *helper);
AstTree *astTreeParseParenthesis(ParserNode *parserNode, AstTreeHelper *helper);

AstTreeFunction *getFunction(AstTree *value);
bool isConst(AstTree *tree, AstTreeHelper *helper);
AstTree *makeTypeOf(AstTree *value);
bool typeIsEqual(const AstTree *type0, const AstTree *type1);
AstTree *getValue(AstTree *tree, AstTreeSetTypesHelper helper);

bool isCircularDependencies(AstTreeHelper *helper, AstTreeVariable *variable);
bool isCircularDependenciesBack(AstTreeHelper *helper,
                                AstTreeVariable *variable, AstTree *tree,
                                AstTreeVariables *checkedVariables);
bool isCircularDependenciesVariable(AstTreeHelper *helper,
                                    AstTreeVariable *toBeFound,
                                    AstTreeVariable *currentVariable,
                                    AstTreeVariables *checkedVariables);

bool setAllTypesRoot(AstTreeRoot *root, AstTreeHelper *helper);
bool setAllTypes(AstTree *tree, AstTreeSetTypesHelper helper,
                 AstTreeFunction *function);
bool setTypesValueBool(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueInt(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueFloat(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueNull(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesFunction(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesPrintU64(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesReturn(AstTree *tree, AstTreeSetTypesHelper helper,
                    AstTreeFunction *function);
bool setTypesTypeFunction(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesFunctionCall(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesVariable(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorAssign(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorInfix(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorInfixWithRet(AstTree *tree, AstTree *retType,
                                  AstTreeSetTypesHelper helper);
bool setTypesOperatorUnary(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorPointer(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorAddress(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorDereference(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesVariableDefine(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesIf(AstTree *tree, AstTreeSetTypesHelper helper,
                AstTreeFunction *function);
bool setTypesWhile(AstTree *tree, AstTreeSetTypesHelper helper,
                   AstTreeFunction *function);
bool setTypesScope(AstTree *tree, AstTreeSetTypesHelper helper,
                   AstTreeFunction *function);
bool setTypesComptime(AstTree *tree, AstTreeSetTypesHelper helper);

bool setTypesAstVariable(AstTreeVariable *variable,
                         AstTreeSetTypesHelper helper);
bool setTypesAstInfix(AstTreeInfix *infix, AstTreeSetTypesHelper helper);
